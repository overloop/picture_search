#ifndef INDEXWORKER_H
#define INDEXWORKER_H

#include <QObject>

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
    void filesAnalyzed(const QStringList& files, const QList<int>& hs, const QList<int>& ss, const QList<int>& ls);

public slots:
    
    void scanDirectories(const QStringList& toAdd, const QStringList& toRemove);
    void filesUnindexed(const QStringList& files);
    void setPreviewDir(const QString& previewDir);
};

#endif // INDEXWORKER_H
