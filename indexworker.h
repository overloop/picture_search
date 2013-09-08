#ifndef INDEXWORKER_H
#define INDEXWORKER_H

#include <QObject>
#include <QTime>

#include "imagestatistics.h"

class IndexWorker : public QObject
{
    Q_OBJECT
public:
    explicit IndexWorker(QObject *parent = 0);

protected:
    QString m_previewDir;
    QTime m_time;
    int m_total;
    int m_done;

    static const int CHUNK_SIZE = 100;

signals:

    void progress(int value);
    void status(const QString& text);

    void filesScaned(const QStringList& files);
    void filesAnalyzed(const ImageStatisticsList& files, bool lastChunk, const QTime& time);

public slots:
    
    void scanDirectories(const QStringList& toAdd, const QStringList& toRemove);
    void filesUnindexed(const QStringList& files);
    void openDatabase(const QStringList& settings);
};

#endif // INDEXWORKER_H
