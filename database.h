#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
class DatabaseSettings;

class Database
{
public:

    static QSqlDatabase open(const DatabaseSettings& settings);
    static bool tablesExist();
    static void createTables(const QString& driver);

private:

};

#endif // DATABASE_H
