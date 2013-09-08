#ifndef DATABASEWORKER_H
#define DATABASEWORKER_H

#include <QObject>
#include <QPair>

#include "imagestatistics.h"

class DatabaseWorker : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseWorker(QObject *parent = 0);
    
protected:
    void createTables();
    QString m_previewDir;
    int m_total, m_done;

signals:
    void databaseOpened(bool ok, const QString& error);
    void filesUnindexed(const QStringList& files);
    void error(const QString& error);
    void directoriesSelected(const QStringList& dirs);
    void filesFound(const ImageStatisticsList& files);

    void status(const QString& text);

public slots:
    void scanDirectories(const QStringList& toAdd, const QStringList& toRemove);
    void openDatabase(const QStringList& settings);
    void selectDirectories();
    void findFiles(const QColor& c, int deviation);

    void filesScaned(const QStringList& files);
    void filesAnalyzed(const ImageStatisticsList& files, bool lastChunk, const QTime& time);


};

#endif // DATABASEWORKER_H
