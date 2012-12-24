#ifndef SQLQUERY_H
#define SQLQUERY_H

#include <QSqlQuery>
#include <QMutex>

class SqlQuery : public QSqlQuery
{
public:
    explicit SqlQuery();
    ~SqlQuery();
protected:
    static QMutex mutex;
};

#endif // SQLQUERY_H
