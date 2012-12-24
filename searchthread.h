#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include <QThread>
#include <QMutex>
#include <QPixmap>
#include <QWaitCondition>
#include <QPair>

class SearchThread : public QThread
{
    Q_OBJECT
public:
    typedef QPair<QString,QImage> SearchResult;
    explicit SearchThread(QObject *parent = 0);
    ~SearchThread();

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

signals:
    void found();

public slots:
    void search(QColor color, int deviation);
};

#endif // SEARCHTHREAD_H
