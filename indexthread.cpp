#include "indexthread.h"

#include <QDir>

#include "indexjob.h"

IndexThread::IndexThread(QObject *parent) :
    QThread(parent),
    m_abort(false)/*,
    m_jobIsRunning(false)*/
{

}

IndexThread::~IndexThread()
{
    mutex.lock();
    m_abort = true;
    waitCondition.wakeOne();
    mutex.unlock();
    wait();
}

void IndexThread::addDirs(const QList<QPair<QString,bool> >& dirs)
{
    m_jobs.queueAddDirs(dirs);
    startOrResume();
}

void IndexThread::removeDirs(const QStringList& dirs)
{
    m_jobs.queueRemoveDirs(dirs);
    startOrResume();
}

void IndexThread::startOrResume()
{
    if (!isRunning())
    {
        start(LowPriority);
    } else {
        waitCondition.wakeOne();
    }
    emit indexStarted();
}

/*virtual*/
void IndexThread::run()
{
    while (true) {

        if (m_jobs.isEmpty())
        {
            //setJobRunning(false);
            mutex.lock();
            emit indexStoped();
            waitCondition.wait(&mutex);
            mutex.unlock();
        }
        else
        {
            //setJobRunning(true);
            IndexJob* job = m_jobs.takeFirst();
            while ((!job->isDone()) && (!m_abort))
            {
                int i = job->make();
                if (i>0) emit progress(i);
            }
            m_jobs.acquireResult(job);
            emit progress(0);
        }

        if (m_abort)
            return;

        /*QStringList dirs;
        QStringList files;
        int n;

        mutex.lock();
        dirs = m_dirs;
        files = m_files;
        m_dirs.clear();
        m_files.clear();
        mutex.unlock();

        n = dirs.size();
        for (int i=0;i<n;i++)
            files.append(addFilesToQueue(dirs.at(i),(i+1)*1000/n));

        n = files.size();
        for (int i=0;i<n;i++)
            addFileToIndex(files.at(i),(i+1)*1000/n);

        mutex.lock();
        if (m_dirs.size()<1 && m_files.size()<1)
            waitCondition.wait(&mutex);
        mutex.unlock();*/
    }
}

/*
QStringList IndexThread::addFilesToQueue(const QString& d, int progressValue)
{
    QDir dir(d);
    QStringList files = dir.entryList( QStringList() << "*.png" << "*.jpg" << "*.gif" << "*.jpeg", QDir::Files );
    emit progress(progressValue);
    return files;
}

void IndexThread::addFileToIndex(const QString& file, int progressValue)
{

}*/
