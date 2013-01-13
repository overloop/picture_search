#include "database.h"

#include "sqlquery.h"
#include "databasesettings.h"

/*static*/
bool Database::tablesExist()
{
    SqlQuery q;
    q.prepare("SELECT count(*) FROM directory JOIN file JOIN color");
    return q.exec();
}

/*static*/
void Database::createTables(const QString& driver)
{
    QStringList queries;

    if (driver == QString("QMYSQL"))
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
                   "preview blob, "
                   "PRIMARY KEY (file_id) "
                   ") ";
    } else if (driver == QString("QSQLITE")) {

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
                   "preview blob "
                   ") ";

    } else {
        Q_ASSERT( QString("Driver %1 not supported for this application.").arg(driver).isEmpty() );
    }

    while (!queries.isEmpty())
    {
        SqlQuery q;
        q.prepare(queries.takeFirst());
        Q_ASSERT(q.exec());
    }

}

/*static*/
QSqlDatabase Database::open(const DatabaseSettings& settings)
{
    QSqlDatabase db = QSqlDatabase::addDatabase(settings.at(DatabaseSettings::DRIVER));

    if (!settings.at(DatabaseSettings::DATABASE).isEmpty())
        db.setDatabaseName(settings.at(DatabaseSettings::DATABASE));

    if (!settings.at(DatabaseSettings::HOST).isEmpty())
        db.setHostName(settings.at(DatabaseSettings::HOST));

    if (!settings.at(DatabaseSettings::USER).isEmpty())
        db.setUserName(settings.at(DatabaseSettings::USER));

    if (!settings.at(DatabaseSettings::PASS).isEmpty())
        db.setPassword(settings.at(DatabaseSettings::PASS));

    //Q_ASSERT(db.open());

    db.open();

    return db;
}
