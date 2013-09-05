#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include <QThread>
#include <QMutex>
#include <QPixmap>
#include <QWaitCondition>
#include <QPair>
#include <QSqlDatabase>

#include "databasesettings.h"

class SearchThread : public QThread
{
    Q_OBJECT
public:
    typedef QPair<QString,QString> SearchResult;
    explicit SearchThread(QObject *parent = 0);
    ~SearchThread();
    void openDatabase(const DatabaseSettings& settings);

    QList<SearchResult> result();

protected:
    void run();

    QMutex mutex;
    QWaitCondition waitCondition;
    QList<SearchResult> m_result;
    bool m_searchRequested;
    QColor m_color;
    int m_deviation;
    bool m_abort;
    DatabaseSettings m_settings;
    QSqlDatabase m_db;

signals:
    void found();

public slots:
    void search(QColor color, int deviation);
};

#endif // SEARCHTHREAD_H
