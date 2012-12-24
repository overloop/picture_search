#include "indexjobs.h"

#include "indexjob.h"

IndexJob* IndexJobs::takeFirst()
{
    QMutexLocker locker(&mutex);

    if (!m_directoriesToAdd.isEmpty())
    {
        QPair<QString, bool> dir = m_directoriesToAdd.takeFirst();
        IndexJob* job = new IndexJobAddDirectory(dir.first,dir.second);
        return job;
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

    return 0;
}

void IndexJobs::acquireResult(IndexJob* job)
{
    QMutexLocker locker(&mutex);

    QStringList result = job->result();

    switch (job->type())
    {
    case IndexJob::AddDirectory:
        m_directoriesToScan.append(result);
        break;
    case IndexJob::ScanDirectories:
        m_filesToAdd.append(result);
        break;
    }
    delete job;
}
