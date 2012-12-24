#ifndef INDEXJOB_H
#define INDEXJOB_H

#include <QStringList>
#include <QPair>

class IndexJob
{
public:
    enum Type {
        AddDirectory,
        ScanDirectories,
        AddFiles
    };

    IndexJob() : m_done(false), m_index(0) {}

    virtual int type() = 0;
    virtual bool isDone() { return m_done;}
    virtual int make() = 0;
    virtual QStringList result() {return QStringList();}

protected:
    bool m_done;
    int m_index;
};

class IndexJobAddDirectory : public IndexJob
{
public:
    IndexJobAddDirectory(const QString& dir, bool subdirs) : m_dir(dir), m_subdirs(subdirs) {}
    int type() {return AddDirectory;}
    int make();
    bool hasRecord(const QString& path);
    QStringList result() {return m_dirs;}
private:
    QStringList m_dirs;
    QString m_dir;
    bool m_subdirs;
};

class IndexJobScanDirectories : public IndexJob
{
public:
    IndexJobScanDirectories(const QStringList& dirs) : m_dirs(dirs) {}
    int type() {return ScanDirectories;}
    int make();
    QStringList result() {return m_files;}
protected:
    QStringList m_dirs;
    QStringList m_files;
};

class IndexJobAddFiles : public IndexJob
{
public:
    IndexJobAddFiles(const QStringList& files) : m_files(files) {}
    int type() {return AddFiles;}
    int make();
    bool hasRecord(const QString path, int directoryId);
    int directoryId(const QString& path);
protected:
    QStringList m_files;
    QPair<QString,int> m_dirid;
};

#endif // INDEXJOB_H
