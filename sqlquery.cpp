#include "sqlquery.h"

/*static*/
QMutex SqlQuery::mutex;

SqlQuery::SqlQuery()
{
    mutex.lock();
}

SqlQuery::SqlQuery(QSqlDatabase db) : QSqlQuery(db)
{
    mutex.lock();
}

SqlQuery::~SqlQuery()
{
    mutex.unlock();
}

