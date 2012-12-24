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

    void addDir(const QString& dir, bool subdirs) {QMutexLocker locker(&mutex); m_directoriesToAdd.append(QPair<QString,bool>(dir,subdirs));}
    bool isEmpty() {QMutexLocker locker(&mutex); return m_directoriesToAdd.isEmpty() && m_directoriesToScan.isEmpty() && m_filesToAdd.isEmpty(); }
    IndexJob* takeFirst();
    void acquireResult(IndexJob* job);

protected:
    QMutex mutex;

    QList< QPair<QString,bool> > m_directoriesToAdd;
    QStringList m_directoriesToScan;
    QStringList m_filesToAdd;

};

#endif // INDEXJOBS_H
