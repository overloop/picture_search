#include "indexthread.h"

#include <QDir>

#include "indexjob.h"

IndexThread::IndexThread(QObject *parent) :
    QThread(parent),
    m_abort(false),
    m_jobIsRunning(false)
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

void IndexThread::addDir(const QString& d, bool subdirs)
{
    m_jobs.addDir(d,subdirs);

    /*QStringList dirs;
    QStringList dirsResult;
    dirs.append(QDir(d));

    if (subdirs)
    {
        do {
            QString dirAsString = dirs.takeFirst();
            QDir dir(dirAsString);
            QStringList subdirNames = dir.entryList( QDir::Dirs );
            QString subdirName;
            foreach(subdirName,subdirNames)
            {
                QString path = dir.filePath(subdirName);
                dirs.append(path);
                dirsResult.append(path);
            }
        } while (dirs.size() > 0);
    }
    else
    {
        dirsResult.append(d);
    }

    mutex.lock();
    m_dirs.append(dirsResult);
    mutex.unlock();*/

    if (!isRunning())
    {
        start(LowPriority);
    } else {
        waitCondition.wakeOne();
    }
    emit indexationStarted();
}

/*virtual*/
void IndexThread::run()
{
    while (true) {

        if (m_jobs.isEmpty())
        {
            setJobRunning(false);
            mutex.lock();
            emit indexationFinished();
            waitCondition.wait(&mutex);
            mutex.unlock();
        }
        else
        {
            setJobRunning(true);
            IndexJob* job = m_jobs.takeFirst();
            while ((!job->isDone()) && (!m_abort))
            {
                int i = job->make();
                if (i>0) emit progress(i);
            }
            m_jobs.acquireResult(job);
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
