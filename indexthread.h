#ifndef INDEXTHREAD_H
#define INDEXTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QStringList>

#include "indexjobs.h"

class IndexThread : public QThread
{
    Q_OBJECT
public:
    explicit IndexThread(QObject *parent = 0);
    ~IndexThread();

    //bool isJobRunning() {return m_jobIsRunning;}

protected:
    virtual void run();

    //QStringList addFilesToQueue(const QString& dir, int progressValue);
    //void addFileToIndex(const QString& file, int progressValue);

    QMutex mutex;
    QWaitCondition waitCondition;
    IndexJobs m_jobs;
    volatile bool m_jobIsRunning;
    volatile bool m_abort;

    //bool setJobRunning(bool state) { QMutexLocker locker(&mutex); m_jobIsRunning = state; }

signals:
    void progress(int);
    void indexStarted();
    void indexStoped();
    
public slots:
    void addDir(const QString& dir, bool subdirs = true);
};

#endif // INDEXTHREAD_H
