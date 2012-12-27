#include "databasesettings.h"
#include <QSettings>
#include "sqlquery.h"
#include <QStringList>

DatabaseSettings::DatabaseSettings()
{

}

QSqlDatabase openDb(const DatabaseSettings& settings) {

    QSqlDatabase db = QSqlDatabase::addDatabase(settings.driver);

    if (!settings.dbName.isEmpty())
        db.setDatabaseName(settings.dbName);

    if (!settings.hostName.isEmpty())
        db.setHostName(settings.hostName);

    if (!settings.dbUser.isEmpty())
        db.setUserName(settings.dbUser);

    if (!settings.dbPass.isEmpty())
        db.setPassword(settings.dbPass);

    //Q_ASSERT(db.open());

    db.open();

    return db;
}

bool tablesExist()
{
    SqlQuery q;
    q.prepare("SELECT count(*) FROM directory JOIN file JOIN color");
    return q.exec();
}

void createTables(const QString& driver)
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
DatabaseSettings DatabaseSettings::defaults(const QString& driver)
{
    QSettings settings("settings.ini",QSettings::IniFormat);

    if (driver == QString("QMYSQL") || driver == QString("QSQLITE"))
    {
        settings.beginGroup(driver);
        return DatabaseSettings(driver,
        settings.value("host").toString(),
        settings.value("database").toString(),
        settings.value("user").toString(),
        settings.value("pass").toString());
    } else {

    }
    return DatabaseSettings();
}

/*static*/
void DatabaseSettings::setDefaults(const DatabaseSettings& settings)
{
    QSettings s("settings.ini",QSettings::IniFormat);
    QString driver = settings.driver;

    if (driver == QString("QMYSQL") || driver == QString("QSQLITE"))
    {
        s.beginGroup(driver);

        s.setValue("host",settings.hostName);
        s.setValue("database",settings.dbName);
        s.setValue("user",settings.dbUser);
        s.setValue("pass",settings.dbPass);

    }

}

