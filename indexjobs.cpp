#include "indexjobs.h"

#include "indexjob.h"

IndexJob* IndexJobs::takeFirst()
{
    QMutexLocker locker(&mutex);

    if (!m_directoriesToAdd.isEmpty())
    {
        /*QPair<QString, bool> dir = m_directoriesToAdd.takeFirst();
        IndexJob* job = new IndexJobAddDirectory(dir.first,dir.second);
        return job;*/

        QList<QPair<QString, bool> > dirs = m_directoriesToAdd;
        m_directoriesToAdd.clear();
        return new IndexJobAddDirectories(dirs);
    }

    if (!m_directoriesToScan.isEmpty())
    {
        QStringList dirs = m_directoriesToScan;
        m_directoriesToScan.clear();
        return new IndexJobScanDirectories(dirs);
    }

    if (!m_filesToAdd.isEmpty())
    {
        QStringList files = m_filesToAdd;
        m_filesToAdd.clear();
        return new IndexJobAddFiles(files);
    }

    if (!m_directoriesToRemove.isEmpty())
    {
        QStringList dirs = m_directoriesToRemove;
        m_directoriesToRemove.clear();
        return new IndexJobRemoveDirectories(dirs);
    }

    return 0;
}

void IndexJobs::acquireResult(IndexJob* job)
{
    QMutexLocker locker(&mutex);

    QStringList result = job->result();

    switch (job->type())
    {
    case IndexJob::ScanDirectories:
        m_filesToAdd.append(result);
        break;
    case IndexJob::AddDirectories:
        m_directoriesToScan.append(result);
        break;
    }
    delete job;
}

void IndexJobs::queueAddDirs(const QList<QPair<QString,bool> >& dirs)
{
    QMutexLocker locker(&mutex);
    m_directoriesToAdd.append(dirs);
}

void IndexJobs::queueRemoveDirs(const QStringList& dirs)
{
    QMutexLocker locker(&mutex);
    m_directoriesToRemove.append(dirs);
}

bool IndexJobs::isEmpty()
{
    QMutexLocker locker(&mutex);
    return m_directoriesToAdd.isEmpty() && m_directoriesToScan.isEmpty() && m_filesToAdd.isEmpty() && m_directoriesToRemove.isEmpty();
}
