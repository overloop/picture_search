#include "database.h"

#include "sqlquery.h"
#include "databasesettings.h"

#include <QDebug>
#include <QSqlError>

/*static*/
bool Database::tablesExist(QSqlDatabase db)
{
    SqlQuery q(db);
    //q.prepare("SELECT count(*) FROM directory JOIN file JOIN color");
    q.prepare("select * from directory limit 1");
    return q.exec();
}

/*static*/
void Database::createTables(QSqlDatabase db)
{
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
        Q_ASSERT( QString("Driver %1 not supported for this application.").arg(db.driverName()).isEmpty() );
    }

    while (!queries.isEmpty())
    {
        SqlQuery q(db);
        q.prepare(queries.takeFirst());
        if (!q.exec())
            qDebug() << q.lastError().text();
    }

}

/*static*/
QSqlDatabase Database::open(const DatabaseSettings& settings, const QString& connectionName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase(settings.at(DatabaseSettings::Driver),connectionName);

    if (!settings.at(DatabaseSettings::Database).isEmpty())
        db.setDatabaseName(settings.at(DatabaseSettings::Database));

    if (!settings.at(DatabaseSettings::Host).isEmpty())
        db.setHostName(settings.at(DatabaseSettings::Host));

    if (!settings.at(DatabaseSettings::User).isEmpty())
        db.setUserName(settings.at(DatabaseSettings::User));

    if (!settings.at(DatabaseSettings::Pass).isEmpty())
        db.setPassword(settings.at(DatabaseSettings::Pass));

    db.open();

    return db;
}
