#ifndef INDEXTHREAD_H
#define INDEXTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QStringList>

#include "indexjobs.h"
class DatabaseSettings;

class IndexThread : public QThread
{
    Q_OBJECT
public:
    explicit IndexThread(QObject *parent = 0);
    ~IndexThread();

    //bool isJobRunning() {return m_jobIsRunning;}

    void addDirs(const QList<QPair<QString,bool> >& dirs);
    void removeDirs(const QStringList& dirs);
    void openDatabase(const DatabaseSettings& settings);

protected:
    virtual void run();
    void startOrResume();

    //QStringList addFilesToQueue(const QString& dir, int progressValue);
    //void addFileToIndex(const QString& file, int progressValue);

    QMutex mutex;
    QWaitCondition waitCondition;
    IndexJobs m_jobs;
    //volatile bool m_jobIsRunning;
    //bool m_abort;

    //bool setJobRunning(bool state) { QMutexLocker locker(&mutex); m_jobIsRunning = state; }

signals:
    void progress(int);
    void indexStarted();
    void indexStoped();
    void databaseOpened(QString error);
    
public slots:

};

#endif // INDEXTHREAD_H
