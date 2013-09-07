#ifndef INDEXWORKER_H
#define INDEXWORKER_H

#include <QObject>

#include "imagestatistics.h"

class IndexWorker : public QObject
{
    Q_OBJECT
public:
    explicit IndexWorker(QObject *parent = 0);

protected:
    QString m_previewDir;

signals:

    void progress(int value);

    void filesScaned(const QStringList& files);
    void filesAnalyzed(const QList<ImageStatistics>& files);

public slots:
    
    void scanDirectories(const QStringList& toAdd, const QStringList& toRemove);
    void filesUnindexed(const QStringList& files);
    void openDatabase(const QStringList& settings);
};

#endif // INDEXWORKER_H
