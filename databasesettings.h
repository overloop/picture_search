#ifndef DATABASESETTINGS_H
#define DATABASESETTINGS_H

#include <QString>
#include "QSqlDatabase"

class DatabaseSettings {
public:
    DatabaseSettings();
    DatabaseSettings(const QString& driver, const QString& hostName, const QString& dbName, const QString& dbUser, const QString& dbPass)
        : driver(driver), hostName(hostName), dbName(dbName), dbUser(dbUser), dbPass(dbPass) {}

    QString driver;
    QString hostName;
    QString dbName;
    QString dbUser;
    QString dbPass;

    static DatabaseSettings defaults(const QString& driver);
    static void setDefaults(const DatabaseSettings& settings);

};

//let it be here
QSqlDatabase openDb(const DatabaseSettings& settings);
void createTables(const QString& driver);
bool tablesExist();

#endif // DATABASESETTINGS_H
