#include "indexjob.h"

#include <QDir>
#include "sqlquery.h"
#include <QVariant>
#include <QBuffer>
#include <QImage>

#include "colorextractorsimple.h"
#include "colorextractorneuquant.h"

int IndexJobScanDirectories::make()
{
    if (m_done)
        return 1000;
    QString dirAsString = m_dirs.at(m_index);

    QDir dir(dirAsString);
    QStringList filters = QStringList() << "*.png" << "*.gif" << "*.jpg" << "*.jpeg";

    QStringList fileNames = dir.entryList(filters, QDir::Files);
    QString fileName;
    foreach(fileName,fileNames)
        m_files.append(dir.filePath(fileName));

    m_index++;
    if (m_index == m_dirs.size())
        m_done = true;
    return (m_index*1000/m_dirs.size());
}

int IndexJobAddFiles::make()
{
    if (m_done)
        return 1000;
    QString fileName = m_files.at(m_index);
    QRegExp regexp("(.*)(/)(.*)");

    if (fileName.indexOf(regexp)>-1)
    {
        QString dir = regexp.cap(1);
        QString name = regexp.cap(3);
        int id = directoryId(dir);
        Q_ASSERT(id!=-1);
        if (!hasRecord(name,id))
        {
            QImage image(fileName);
            ColorExtractorSimple extractor(image);
            QList<QColor> common = extractor.extract();
            QColor color;

            QImage preview = extractor.scaled().scaled(QSize(150,150),Qt::KeepAspectRatio);

            QByteArray previewByteArray;
            QBuffer buf(&previewByteArray);
            buf.open(QIODevice::WriteOnly);
            preview.save(&buf,"JPEG");
            //extractor.scaled().save(&buf,"JPEG");

            SqlQuery q;
            q.prepare("INSERT INTO file(directory_id,path,preview) VALUES(?,?,?)");
            q.addBindValue(id);
            q.addBindValue(name);
            q.addBindValue(previewByteArray);

            Q_ASSERT(q.exec());

            QVariant file_id = q.lastInsertId();

            foreach(color,common)
            {
                int h,s,l;
                color.getHsl(&h,&s,&l);

                q.prepare("INSERT INTO color(file_id,h,s,l) VALUES(?,?,?,?)");
                q.addBindValue(file_id);
                q.addBindValue(h);
                q.addBindValue(s);
                q.addBindValue(l);
                Q_ASSERT(q.exec());
            }



        }
    }

    m_index++;
    if (m_index == m_files.size())
        m_done = true;
    return (m_index*1000/m_files.size());
}

bool IndexJobAddFiles::hasRecord(const QString path, int directoryId)
{
    SqlQuery q;
    q.prepare("SELECT count(*) FROM file WHERE directory_id=? AND path=?");
    q.addBindValue(directoryId);
    q.addBindValue(path);
    Q_ASSERT(q.exec());
    Q_ASSERT(q.next());
    if (q.value(0).toInt()<1)
    {
        return false;
    }
    return true;
}

int IndexJobAddFiles::directoryId(const QString& path)
{
    if (path == m_dirid.first)
    {
        return m_dirid.second;
    }

    int id = -1;

    SqlQuery q;
    q.prepare("SELECT directory_id FROM directory WHERE path=?");
    q.addBindValue(path);
    Q_ASSERT(q.exec());
    if (q.next())
        id = q.value(0).toInt();

    m_dirid = QPair<QString,int>(path,id);

    return id;
}

int IndexJobRemoveDirectories::make()
{
    int n = m_dirs.size();
    for (int i=0;i<n;i++)
    {
        QString dir = m_dirs.at(i);
        SqlQuery q;
        q.prepare("DELETE FROM color WHERE file_id in "
                  "(SELECT file.file_id "
                  "FROM directory "
                  "JOIN file "
                  "WHERE "
                  "directory.path LIKE ? "
                  "AND file.directory_id = directory.directory_id) ");
        q.addBindValue(dir + QString("/%"));
        Q_ASSERT(q.exec());

        q.prepare("DELETE FROM color WHERE file_id in "
                  "(SELECT file.file_id "
                  "FROM directory "
                  "JOIN file "
                  "WHERE "
                  "directory.path=? "
                  "AND file.directory_id = directory.directory_id) ");
        q.addBindValue(dir);
        Q_ASSERT(q.exec());

        q.prepare("DELETE FROM file WHERE directory_id in "
                  "(SELECT directory_id "
                  "FROM directory "
                  "WHERE "
                  "directory.path LIKE ?) ");
        q.addBindValue(dir + QString("/%"));
        Q_ASSERT(q.exec());

        q.prepare("DELETE FROM file WHERE directory_id in "
                  "(SELECT directory_id "
                  "FROM directory "
                  "WHERE "
                  "directory.path=?) ");
        q.addBindValue(dir);
        Q_ASSERT(q.exec());

        q.prepare("DELETE FROM directory WHERE path LIKE ?");
        q.addBindValue(dir + QString("/%"));
        Q_ASSERT(q.exec());

        q.prepare("DELETE FROM directory WHERE path=?");
        q.addBindValue(dir);
        Q_ASSERT(q.exec());

    }
    m_done = true;
    return 1000;
}

int IndexJobAddDirectories::make()
{
    if (m_done)
        return 1000;

    QPair<QString,bool> oneDir = m_dirs.at(m_index);
    makeOne(oneDir.first,oneDir.second);

    m_index++;
    if (m_index == m_dirs.size())
        m_done = true;
    return (m_index*1000/m_dirs.size());
}

void IndexJobAddDirectories::makeOne(QString oneDir, bool subdirs)
{
    QDir dir(oneDir);
    oneDir = dir.absolutePath();
    QStringList resDirs;

    QStringList dirs;
    dirs.append(oneDir);
    if (subdirs)
    {
        do {
            QString dirAsString = dirs.takeFirst();
            QDir dir(dirAsString);
            QStringList subdirNames = dir.entryList( QDir::Dirs );
            QString subdirName;
            foreach(subdirName,subdirNames)
            {
                if (subdirName == "." || subdirName == "..") continue;
                QString path = dir.filePath(subdirName);
                dirs.append(path);
                resDirs.append(path);
            }
            resDirs.append(dirAsString);
        } while (dirs.size() > 0);
    }
    else
    {
        resDirs.append(oneDir);
    }

    if (!hasRecord(oneDir))
    {
        SqlQuery q;
        q.prepare("INSERT into directory(path,subdirs,user) VALUES(?,?,?)");
        q.addBindValue(oneDir);
        q.addBindValue(subdirs?1:0);
        q.addBindValue(1);
        Q_ASSERT(q.exec());
    }

    if (subdirs)
    {
        QString path;
        foreach(path,resDirs)
        {
            if ((path!=oneDir) && (!hasRecord(path)))
            {
                SqlQuery q;
                q.prepare("INSERT into directory(path,subdirs,user) VALUES(?,?,?)");
                q.addBindValue(path);
                q.addBindValue(false);
                q.addBindValue(0);
                Q_ASSERT(q.exec());
            }
        }
    }
    m_resDirs.append(resDirs);
}

bool IndexJobAddDirectories::hasRecord(const QString& path)
{
    SqlQuery q;
    q.prepare("SELECT count(*) FROM directory WHERE path=?");
    q.addBindValue(path);
    Q_ASSERT(q.exec());
    Q_ASSERT(q.next());
    if (q.value(0).toInt()<1)
    {
        return false;
    }
    return true;
}
