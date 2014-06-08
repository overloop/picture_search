#include "databaseworker.h"

#include "databasesettings.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QFileInfo>
#include <QSqlQuery>
#include <QDir>
#include <QVariant>
#include <QDebug>
#include <QTime>

#define QUERY_EXEC(q) do{ if (!q.exec()) qDebug() << q.lastError().text() << q.lastQuery(); /*qDebug() << q.lastQuery();*/ } while(0)

DatabaseWorker::DatabaseWorker(QObject *parent) :
    QObject(parent), m_total(0), m_done(0)
{

}

void DatabaseWorker::openDatabase(const QStringList& settings) {

    QSqlDatabase db = QSqlDatabase::addDatabase(settings.at(DatabaseSettings::Driver));

    if (!settings.at(DatabaseSettings::Database).isEmpty())
        db.setDatabaseName(settings.at(DatabaseSettings::Database));

    if (!settings.at(DatabaseSettings::Host).isEmpty())
        db.setHostName(settings.at(DatabaseSettings::Host));

    if (!settings.at(DatabaseSettings::User).isEmpty())
        db.setUserName(settings.at(DatabaseSettings::User));

    if (!settings.at(DatabaseSettings::Pass).isEmpty())
        db.setPassword(settings.at(DatabaseSettings::Pass));

    if (db.open()) {
        emit databaseOpened(true,QString());
    } else {
        emit databaseOpened(false,db.lastError().text());
        return;
    }

    m_previewDir = settings.at(DatabaseSettings::PreviewDir);

    QString query;
    QStringList queries;
    queries << "SELECT * FROM file LIMIT 1" << "SELECT * FROM directory LIMIT 1"
            << "SELECT * FROM color LIMIT 1";

    QSqlQuery q(db);
    foreach(query,queries) {
        q.prepare(query);
        if (!q.exec()) {
            createTables();
            break;
        }
    }

}

void DatabaseWorker::selectDirectories() {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        emit error("Database is not opened");
        return ;
    }
    QSqlQuery q(db);

    q.prepare("SELECT path FROM directory WHERE user=1");
    q.exec();

    QStringList dirs;
    while (q.next())
        dirs << q.value(0).toString();

    emit directoriesSelected(dirs);
}

void DatabaseWorker::scanDirectories(const QStringList& toAdd, const QStringList& toRemove) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        emit error("Database is not opened");
        return ;
    }
    QSqlQuery q(db);
    m_done = 0;
    m_total = 0;

    QString dir;
    foreach (dir,toRemove) {
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

    foreach(dir,toAdd) {
        q.prepare("SELECT count(*) FROM directory WHERE path=?");
        q.addBindValue(dir);
        QUERY_EXEC(q);
        if (q.next() && q.value(0).toInt()>0) {
            q.prepare("UPDATE directory SET user=1 WHERE path=?");
            q.addBindValue(dir);
            QUERY_EXEC(q);
        } else {
            q.prepare("INSERT INTO directory(path,user) VALUES(?,1)");
            q.addBindValue(dir);
            QUERY_EXEC(q);
        }
    }


}

void DatabaseWorker::createTables()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        emit error("Database is not opened");
        return ;
    }

    QStringList queries;

    if (db.driverName() == QString("QMYSQL"))
    {
        queries << "CREATE TABLE color ( "
                   "color_id int(11) NOT NULL AUTO_INCREMENT, "
                   "file_id int(11) DEFAULT NULL, "
                   "h int(11) DEFAULT NULL, "
                   "s int(11) DEFAULT NULL, "
                   "l int(11) DEFAULT NULL, "
                   "PRIMARY KEY (color_id) "
                   ") ";
        queries << "CREATE TABLE directory ( "
                   "directory_id int(11) NOT NULL AUTO_INCREMENT, "
                   "path text, "
                   "user int(11) DEFAULT NULL, "
                   "PRIMARY KEY (directory_id) "
                   ") ";
        queries << "CREATE TABLE file ( "
                   "file_id int(11) NOT NULL AUTO_INCREMENT, "
                   "directory_id int(11) DEFAULT NULL, "
                   "path text, "
                   "preview text, "
                   "PRIMARY KEY (file_id) "
                   ") ";
    } else if (db.driverName() == QString("QSQLITE")) {

        queries << "CREATE TABLE color ( "
                   "color_id integer primary key autoincrement, "
                   "file_id integer, "
                   "h integer, "
                   "s integer, "
                   "l integer "
                   ") ";
        queries << "CREATE TABLE directory ( "
                   "directory_id integer primary key autoincrement, "
                   "path text, "
                   "user integer "
                   ") ";
        queries << "CREATE TABLE file ( "
                   "file_id integer primary key autoincrement, "
                   "directory_id integer, "
                   "path text, "
                   "preview text "
                   ") ";

    } else {
        emit error(QString("Driver %1 not supported for this application.").arg(db.driverName()));
        return ;
    }

    QSqlQuery q(db);
    while (!queries.isEmpty())
    {
        q.prepare(queries.takeFirst());
        if (!q.exec()) {
            emit error(QString("Can't create tables in database: ") + q.lastError().text());
        }
    }

}

void DatabaseWorker::filesScaned(const QStringList& files) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        emit error("Database is not opened");
        return ;
    }
    QSqlQuery q(db);

    QStringList unindexed;
    QString file;
    foreach(file,files) {
        QFileInfo t_file(file);
        q.prepare("SELECT count(*) FROM file JOIN directory "
                  "WHERE directory.directory_id=file.directory_id "
                  "AND directory.path=? "
                  "AND file.path=? ");
        q.addBindValue(t_file.dir().absolutePath());
        q.addBindValue(t_file.fileName());
        QUERY_EXEC(q);
        if (q.next() && q.value(0).toInt()<1)
        {
            unindexed << file;
        } else {
            --m_total;
        }
    }

    //if (unindexed.size()>0)
    emit filesUnindexed(unindexed);
}

void DatabaseWorker::filesAnalyzed(const ImageStatisticsList &files, bool lastChunk, const QTime &time) {

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        emit error("Database is not opened");
        return ;
    }
    QSqlQuery q(db);

    ImageStatistics stat;
    foreach(stat,files) {
        QFileInfo file(stat.file);

        QVariant directoryId;
        QString dir = file.dir().absolutePath();
        QString path = file.fileName();

        q.prepare("SELECT directory_id FROM directory WHERE path=?");
        q.addBindValue(dir);
        QUERY_EXEC(q);

        if (q.next()) {
            directoryId = q.value(0);
        } else {
            q.prepare("INSERT INTO directory(path,user) VALUES(?,?)");
            q.addBindValue(dir);
            q.addBindValue(0);
            QUERY_EXEC(q);
            directoryId = q.lastInsertId();
        }

        q.prepare("INSERT INTO file(directory_id,path,preview) VALUES(?,?,?)");
        q.addBindValue(directoryId);
        q.addBindValue(path);
        q.addBindValue(stat.preview);
        q.exec();

        QVariant fileId = q.lastInsertId();

        int n = stat.colors.size();
        for (int i=0;i<n;i++) {
            q.prepare("INSERT INTO color(file_id,h,s,l) VALUES(?,?,?,?)");
            q.addBindValue(fileId);
            q.addBindValue(stat.colors.at(i).hue());
            q.addBindValue(stat.colors.at(i).saturation());
            q.addBindValue(stat.colors.at(i).lightness());
            QUERY_EXEC(q);
        }
    }

    if (lastChunk)
        emit status( QString("Index updated in %1s").arg(QString::number((double)time.elapsed()/1000.0,'f',2)) );

}

void DatabaseWorker::findFiles(const QColor& c, int deviation) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        emit error("Database is not opened");
        return ;
    }
    QSqlQuery q(db);

    /*q.prepare("SELECT file.file_id,directory.path,file.path,preview "
              "FROM file JOIN directory JOIN color "
              "WHERE file.directory_id=directory.directory_id "
              "AND color.file_id=file.file_id "
              "AND (abs(h-?)*2+abs(s-?)+abs(l-?))<? "
              "GROUP BY file.file_id "
              "LIMIT 300 ");
    q.addBindValue(c.hue());
    q.addBindValue(c.saturation());
    q.addBindValue(c.lightness());
    q.addBindValue(deviation);
    QUERY_EXEC(q);*/

    QHash<int,QList<QColor> > colors;
    QHash<int,QString> paths;
    QHash<int,QString> previews;

    q.prepare("SELECT DISTINCT file_id from color where (abs(h-?)*2+abs(s-?)+abs(l-?))<? LIMIT 300");
    q.addBindValue(c.hue());
    q.addBindValue(c.saturation());
    q.addBindValue(c.lightness());
    q.addBindValue(deviation);
    QUERY_EXEC(q);

    QStringList fileIds;
    while (q.next()) {
        int fileId = q.value(0).toInt();
        fileIds << QString::number(fileId);
        colors.insert(fileId, QList<QColor>());
    }

    q.prepare("SELECT file.file_id,directory.path,file.path,preview,h,s,l "
              "FROM file JOIN directory JOIN color "
              "WHERE file.directory_id=directory.directory_id "
              "AND color.file_id=file.file_id AND file.file_id in (" + fileIds.join(",") + ") ORDER BY file.file_id");
    QUERY_EXEC(q);

    while (q.next()) {
        int id = q.value(0).toInt();
        QString path = q.value(1).toString() + "/" + q.value(2).toString();
        QString preview = m_previewDir + QDir::separator() + q.value(3).toString();
        int h = q.value(4).toInt();
        int s = q.value(5).toInt();
        int l = q.value(6).toInt();
        QColor color = QColor::fromHsl(h,s,l);

        colors[id].append(color);
        paths[id] = path;
        previews[id] = preview;
    }

    ImageStatisticsList result;
    QList<int> ids = paths.keys();
    int id;
    foreach(id,ids) {
        result.append(ImageStatistics(id,paths[id],previews[id],colors[id]));
    }

    /*ImageStatisticsList result;
    while (q.next()) {
        int id = q.value(0).toInt();
        QString path = q.value(1).toString() + "/" + q.value(2).toString();
        QString preview = m_previewDir + QDir::separator() + q.value(3).toString();
        result.append(ImageStatistics(id,path,preview));
    }*/
    emit filesFound(result);
}
