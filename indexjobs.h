#ifndef INDEXJOBS_H
#define INDEXJOBS_H

class IndexJob;

#include <QMutex>
#include <QList>
#include <QPair>
#include <QStringList>

#include "databasesettings.h"

class IndexJobs
{
public:

    bool isEmpty();
    IndexJob* takeFirst();
    void acquireResult(IndexJob* job);

    void queueAddDirs(const QList<QPair<QString,bool> >& dirs);
    void queueRemoveDirs(const QStringList& dirs);
    void queueOpenDatabase(const DatabaseSettings& settings);
    QString error();

protected:
    QMutex mutex;

    QList< QPair<QString,bool> > m_directoriesToAdd;
    QStringList m_directoriesToScan;
    QStringList m_filesToAdd;
    QStringList m_directoriesToRemove;
    DatabaseSettings m_settings;
    QString m_error;

};

#endif // INDEXJOBS_H
