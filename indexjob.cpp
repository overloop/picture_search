#include "indexjob.h"

#include <QDir>
#include "sqlquery.h"
#include <QVariant>
#include <QBuffer>
#include <QImage>
#include "imageanalyzer.h"

int IndexJobAddDirectory::make()
{
    QDir dir(m_dir);
    m_dir = dir.absolutePath();

    QStringList dirs;
    dirs.append(m_dir);
    if (m_subdirs)
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
                m_dirs.append(path);
            }
            m_dirs.append(dirAsString);
        } while (dirs.size() > 0);
    }
    else
    {
        m_dirs.append(m_dir);
    }

    if (!hasRecord(m_dir))
    {
        SqlQuery q;
        q.prepare("INSERT into directory(path,subdirs,user) VALUES(?,?,?)");
        q.addBindValue(m_dir);
        q.addBindValue(m_subdirs);
        q.addBindValue(1);
        Q_ASSERT(q.exec());
    }

    if (m_subdirs)
    {
        QString path;
        foreach(path,m_dirs)
        {
            if ((path!=m_dir) && (!hasRecord(path)))
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

    m_done = true;
    return 1000;
}

bool IndexJobAddDirectory::hasRecord(const QString& path)
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
            ImageAnalyzer analyzer(image);
            QList<QColor> common = analyzer.commonColors();
            QColor color;

            QByteArray previewByteArray;
            QBuffer buf(&previewByteArray);
            buf.open(QIODevice::WriteOnly);
            analyzer.scaled().save(&buf,"JPEG");

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
