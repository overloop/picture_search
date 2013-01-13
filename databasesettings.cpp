#include "databasesettings.h"

DatabaseSettings::DatabaseSettings()
{
    for (int i=0;i<SIZE;i++)
        append(QString());
}
