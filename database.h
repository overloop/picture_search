#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
class DatabaseSettings;

class Database
{
public:

    static QSqlDatabase open(const DatabaseSettings& settings, const QString &connectionName);
    static bool tablesExist(QSqlDatabase db);
    static void createTables(QSqlDatabase db);

private:

};

#endif // DATABASE_H
