#ifndef DATABASESETTINGS_H
#define DATABASESETTINGS_H

#include <QStringList>

class DatabaseSettings : public QStringList
{
public:
    DatabaseSettings()
    {
        for (int i=0;i<SIZE;i++)
            append(QString());
    }
    enum INDEX {
        DRIVER = 0,
        HOST,
        DATABASE,
        USER,
        PASS,
        SIZE
    };
};


#endif // DATABASESETTINGS_H
