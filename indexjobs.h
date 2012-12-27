#ifndef INDEXJOBS_H
#define INDEXJOBS_H

#include <QMutex>
#include <QList>
#include <QPair>
#include <QStringList>

class IndexJob;

class IndexJobs
{
public:

    bool isEmpty();
    IndexJob* takeFirst();
    void acquireResult(IndexJob* job);

    void queueAddDirs(const QList<QPair<QString,bool> >& dirs);
    void queueRemoveDirs(const QStringList& dirs);

protected:
    QMutex mutex;

    QList< QPair<QString,bool> > m_directoriesToAdd;
    QStringList m_directoriesToScan;
    QStringList m_filesToAdd;
    QStringList m_directoriesToRemove;

};

#endif // INDEXJOBS_H
