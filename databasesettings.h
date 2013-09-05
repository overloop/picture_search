#ifndef DATABASESETTINGS_H
#define DATABASESETTINGS_H

#include <QStringList>

class DatabaseSettings : public QStringList
{
public:
    enum Index {
        Driver = 0,
        Host,
        Database,
        User,
        Pass,
        PreviewDir,
        Size
    };
    DatabaseSettings();

    bool isEmpty() const; //hmmmmm
};

#endif // DATABASESETTINGS_H
