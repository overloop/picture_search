#include "databasesettings.h"

DatabaseSettings::DatabaseSettings()
{
    for (int i=0;i<SIZE;i++)
        append(QString());
}

bool DatabaseSettings::isEmpty() const
{
    for (int i=0;i<SIZE;i++)
        if (!at(i).isEmpty())
            return false;
    return true;
}
