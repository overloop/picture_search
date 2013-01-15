#ifndef DATABASESETTINGS_H
#define DATABASESETTINGS_H

#include <QStringList>

class DatabaseSettings : public QStringList
{
public:
    enum INDEX {
        DRIVER = 0,
        HOST,
        DATABASE,
        USER,
        PASS,
        SIZE
    };
    DatabaseSettings();

    bool isEmpty() const; //hmmmmm
};

#endif // DATABASESETTINGS_H
