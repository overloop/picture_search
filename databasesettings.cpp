#include "databasesettings.h"

DatabaseSettings::DatabaseSettings()
{
    for (int i=0;i<Size;i++)
        append(QString());
}

bool DatabaseSettings::isEmpty() const
{
    for (int i=0;i<Size;i++)
        if (!at(i).isEmpty())
            return false;
    return true;
}
