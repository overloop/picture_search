#ifndef DATABASEWORKER_H
#define DATABASEWORKER_H

#include <QObject>

class DatabaseWorker : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseWorker(QObject *parent = 0);
    
protected:
    void createTables();

signals:
    void databaseOpened(bool ok, const QString& error);
    void error(const QString& error);
    void filesUnindexed(const QStringList& files);

public slots:
    void scanDirectories(const QStringList& toAdd, const QStringList& toRemove);
    void openDatabase(const QStringList& settings);
    void filesScaned(const QStringList& files);
    void filesAnalyzed(const QStringList& files, const QList<int>& hs, const QList<int>& ss, const QList<int>& ls);
};

#endif // DATABASEWORKER_H
