#include "searchthread.h"

#include "sqlquery.h"
#include <QVariant>
#include <QImage>
#include <QByteArray>
#include <QBuffer>

SearchThread::SearchThread(QObject *parent) :
    QThread(parent),
    m_searchRequested(false),
    m_abort(false)
{

}

SearchThread::~SearchThread()
{
    mutex.lock();
    m_abort = true;
    mutex.unlock();
    waitCondition.wakeOne();
    wait();
}

void SearchThread::search(QColor color, int deviation)
{
    mutex.lock();
    m_color = color;
    m_deviation = deviation;
    m_searchRequested = true;
    mutex.unlock();

    if (!isRunning())
    {
        start(LowPriority);
    }
    else
    {
        waitCondition.wakeOne();
    }
}

QList<SearchThread::SearchResult> SearchThread::result()
{
    QMutexLocker locker(&mutex);
    return m_result;
}

void SearchThread::run()
{
    while (true) {

        if (m_searchRequested)
        {
            mutex.lock();
            m_searchRequested = false;
            QColor color = m_color;
            int deviation = m_deviation;
            mutex.unlock();

            int h,s,l;
            color.getHsl(&h,&s,&l);

            SqlQuery q;
            q.prepare("SELECT directory.path,file.path,preview FROM file JOIN directory "
                      "JOIN color "
                      "WHERE file.directory_id=directory.directory_id AND "
                      "color.file_id=file.file_id AND "
                      "(abs(h-?)*2+abs(s-?)+abs(l-?))<? "
                      "GROUP BY file.file_id");
            q.addBindValue(h);
            q.addBindValue(s);
            q.addBindValue(l);
            q.addBindValue(deviation);
            bool res = q.exec();
            Q_ASSERT(res);

            QList<SearchResult> result;

            while (q.next() && !m_abort)
            {
                //result.append(SearchResult(q.va));
                QImage preview;
                QByteArray previewByteArray = q.value(2).toByteArray();
                QBuffer buf(&previewByteArray);
                buf.open(QIODevice::ReadOnly);
                preview.load(&buf,"JPEG");

                QString path = q.value(0).toString() + QString("/") + q.value(1).toString();
                result.append(SearchResult(path,preview));
            }

            mutex.lock();
            m_result.clear();
            m_result.append(result);
            mutex.unlock();
            emit found();
        }
        else
        {
            mutex.lock();
            waitCondition.wait(&mutex);
            mutex.unlock();
        }

        if (m_abort)
            return;

    }
}
