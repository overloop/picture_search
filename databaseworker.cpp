#include "databaseworker.h"

#include "databasesettings.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QFileInfo>
#include <QSqlQuery>
#include <QDir>
#include <QVariant>
#include <QDebug>

#define QUERY_EXEC(q) do{ if (!q.exec()) qDebug() << q.lastError().text(); qDebug() << q.lastQuery(); } while(0)

DatabaseWorker::DatabaseWorker(QObject *parent) :
    QObject(parent)
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

    QString query;
    QStringList queries;
    queries << "select * from file limit 1" << "select * from directory limit 1"
            << "select * from color limit 1";

    QSqlQuery q(db);
    foreach(query,queries) {
        q.prepare(query);
        if (!q.exec()) {
            createTables();
            break;
        }
    }

}

void DatabaseWorker::scanDirectories(const QStringList& toAdd, const QStringList& toRemove) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        emit error("Database is not opened");
        return ;
    }
    QSqlQuery q(db);

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
        q.prepare("INSERT INTO directory(path,user) VALUES(?,?)");
        q.addBindValue(dir);
        q.addBindValue(1);
        QUERY_EXEC(q);
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
                   "subdirs int(11) DEFAULT NULL, "
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
                   "subdirs integer, "
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
            emit error(q.lastError().text());
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
        q.prepare("SELECT count(*) FROM file JOIN directory WHERE directory.directory_id=file.directory_id AND directory.path=? AND file.path=?");
        q.addBindValue(t_file.dir().absolutePath());
        q.addBindValue(t_file.completeBaseName());
        QUERY_EXEC(q);
        if (q.next() && q.value(0).toInt()<1)
        {
            unindexed << file;
        }
    }

    if (unindexed.size()>0)
        emit filesUnindexed(unindexed);
}

void DatabaseWorker::filesAnalyzed(const QList<ImageStatistics>& files) {

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
        QString path = file.completeBaseName();

        q.prepare("SELECT directory_id from directory where path=?");
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
            q.addBindValue(stat.h(i));
            q.addBindValue(stat.s(i));
            q.addBindValue(stat.l(i));
            QUERY_EXEC(q);
        }


    }

}
