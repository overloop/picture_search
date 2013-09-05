#include "indexjob.h"

#include <QDir>
#include <QVariant>
#include <QBuffer>
#include <QImage>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QCryptographicHash>

#include "colorextractorsimple.h"
#include "colorextractorneuquant.h"
#include "database.h"

//#define QUERY_EXEC(q) if (!q.exec()) qDebug() << q.lastError().text(); else qDebug() << q.numRowsAffected() << " rows affected; size: " << q.size() << q.lastQuery()
#define QUERY_EXEC(q) do{ if (!q.exec()) qDebug() << q.lastError().text(); qDebug() << q.lastQuery(); } while(0)

/* static */
QSqlDatabase IndexJob::m_database;

int IndexJobOpenDatabase::make()
{
    m_database = Database::open(m_settings,"IndexThread");

    if (!m_database.isOpen())
    {
        m_error = m_database.lastError().text();
        m_done = true;
        return 1000;
    }

    if (!Database::tablesExist(m_database))
        Database::createTables(m_database);

    /*QSqlQuery q(m_database);
    q.prepare("INSERT into file (path) values('test')");
    q.exec();*/

    m_done = true;
    return 1000;
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
            /** @todo разобрать с малформатными картинками */
            /*
               Иногда программа падает не осилив картинку, с этим нужно разбираться чтобы починить
               чтобы она не падала второй раз на том же файле, будем добавлять запись о файле в индекс,
               а потом уже открывать картинку и апдейтом добавлять имя превью файла.
               Если программа упадёт, то пользователь сможет перезапустить её, и продолжить индексацию,
               пропустив злосчастную картинку.
            */

            QString previewFileName = "blank.jpg";

            QSqlQuery q(m_database);
            q.prepare("INSERT INTO file(directory_id,path,preview) VALUES(?,?,?)");
            q.addBindValue(id);
            q.addBindValue(name);
            q.addBindValue(previewFileName);
            QUERY_EXEC(q);

            QVariant fileId = q.lastInsertId();

            QImage image(fileName);
            ColorExtractorSimple extractor(image);
            QList<QColor> common = extractor.extract();

            QImage preview = extractor.scaled().scaled(QSize(150,150),Qt::KeepAspectRatio);

            QByteArray previewByteArray;

            QBuffer buf(&previewByteArray);
            buf.open(QIODevice::WriteOnly);
            preview.save(&buf,"JPEG");

            QString md5 = QString(QCryptographicHash::hash(previewByteArray,QCryptographicHash::Md5).toHex());
            previewFileName = md5 + QString(".jpg");

            QFile file(QDir(m_previewDir).filePath(previewFileName));
            file.open(QIODevice::WriteOnly);
            file.write(previewByteArray);
            file.close();

            q.prepare("update file set preview=? where file_id=?");
            q.addBindValue(previewFileName);
            q.addBindValue(fileId);
            QUERY_EXEC(q);

            //qDebug() << "IndexJobAddFiles::make()";

            QColor color;
            foreach(color,common)
            {
                int h,s,l;
                color.getHsl(&h,&s,&l);

                q.prepare("INSERT INTO color(file_id,h,s,l) VALUES(?,?,?,?)");
                q.addBindValue(fileId);
                q.addBindValue(h);
                q.addBindValue(s);
                q.addBindValue(l);
                QUERY_EXEC(q);
            }
        }
    }

    m_index++;
    if (m_index == m_files.size())
        m_done = true;
    return (m_index*1000/m_files.size());
}

/** @todo cache me */
bool IndexJobAddFiles::hasRecord(const QString path, int directoryId)
{
    //qDebug() << "IndexJobAddFiles::hasRecord(const QString path, int directoryId)";

    QSqlQuery q(m_database);
    q.prepare("SELECT count(*) FROM file WHERE directory_id=? AND path=?");
    q.addBindValue(directoryId);
    q.addBindValue(path);
    QUERY_EXEC(q);
    if (q.next() && q.value(0).toInt()<1)
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

    QSqlQuery q(m_database);
    q.prepare("SELECT directory_id FROM directory WHERE path=?");
    q.addBindValue(path);
    QUERY_EXEC(q);
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
        QSqlQuery q(m_database);
        q.prepare("DELETE FROM color WHERE file_id in "
                  "(SELECT file.file_id "
                  "FROM directory "
                  "JOIN file "
                  "WHERE "
                  "directory.path LIKE ? "
                  "AND file.directory_id = directory.directory_id) ");
        q.addBindValue(dir + QString("/%"));
        QUERY_EXEC(q);

        q.prepare("DELETE FROM color WHERE file_id in "
                  "(SELECT file.file_id "
                  "FROM directory "
                  "JOIN file "
                  "WHERE "
                  "directory.path=? "
                  "AND file.directory_id = directory.directory_id) ");
        q.addBindValue(dir);
        QUERY_EXEC(q);

        q.prepare("DELETE FROM file WHERE directory_id in "
                  "(SELECT directory_id "
                  "FROM directory "
                  "WHERE "
                  "directory.path LIKE ?) ");
        q.addBindValue(dir + QString("/%"));
        QUERY_EXEC(q);

        q.prepare("DELETE FROM file WHERE directory_id in "
                  "(SELECT directory_id "
                  "FROM directory "
                  "WHERE "
                  "directory.path=?) ");
        q.addBindValue(dir);
        QUERY_EXEC(q);

        q.prepare("DELETE FROM directory WHERE path LIKE ?");
        q.addBindValue(dir + QString("/%"));
        QUERY_EXEC(q);

        q.prepare("DELETE FROM directory WHERE path=?");
        q.addBindValue(dir);
        QUERY_EXEC(q);

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
        QSqlQuery q(m_database);
        q.prepare("INSERT into directory(path,subdirs,user) VALUES(?,?,?)");
        q.addBindValue(oneDir);
        q.addBindValue(subdirs?1:0);
        q.addBindValue(1);
        QUERY_EXEC(q);
    }

    if (subdirs)
    {
        QString path;
        foreach(path,resDirs)
        {
            if ((path!=oneDir) && (!hasRecord(path)))
            {
                QSqlQuery q(m_database);
                q.prepare("INSERT into directory(path,subdirs,user) VALUES(?,?,?)");
                q.addBindValue(path);
                q.addBindValue(false);
                q.addBindValue(0);
                QUERY_EXEC(q);
            }
        }
    }
    m_resDirs.append(resDirs);
}

bool IndexJobAddDirectories::hasRecord(const QString& path)
{
    QSqlQuery q(m_database);
    q.prepare("SELECT count(*) FROM directory WHERE path=?");
    q.addBindValue(path);
    QUERY_EXEC(q);
    if (q.next() && q.value(0).toInt()<1)
    {
        return false;
    }
    return true;
}
