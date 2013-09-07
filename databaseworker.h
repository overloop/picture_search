#ifndef DATABASEWORKER_H
#define DATABASEWORKER_H

#include <QObject>

#include "imagestatistics.h"

class DatabaseWorker : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseWorker(QObject *parent = 0);
    
protected:
    void createTables();

signals:
    void databaseOpened(bool ok, const QString& error);
    void filesUnindexed(const QStringList& files);
    void error(const QString& error);

public slots:
    void scanDirectories(const QStringList& toAdd, const QStringList& toRemove);
    void openDatabase(const QStringList& settings);
    void filesScaned(const QStringList& files);
    void filesAnalyzed(const QList<ImageStatistics>& files);
};

#endif // DATABASEWORKER_H
