#include "pixmaplabel.h"

#include <QResizeEvent>
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>

PixmapLabel::PixmapLabel(QWidget *parent) :
    QWidget(parent)
{
    //label = new QLabel(this);
    //label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    //label->setScaledContents(true);
    //this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

void PixmapLabel::resizeEvent(QResizeEvent *event)
{
    //resizeLabel(this->size());
    //qDebug() << event->oldSize() << event->size();
    /*QSize size = event->size();
    if (size.width()>0 && size.height()>0)
        resizeLabel(size);*/

    event->accept();
}

void PixmapLabel::paintEvent(QPaintEvent * event)
{
    QPainter p(this);

    p.fillRect(event->rect(),QColor(Qt::white));

    if (!pixmap.isNull())
    {
        QSize pixmapSize = pixmap.size();
        QSize totalSize = this->size();
        pixmapSize.scale(totalSize,Qt::KeepAspectRatio);
        QSize rest = (totalSize - pixmapSize) / 2;
        QRect rect(rest.width(),rest.height(),pixmapSize.width(),pixmapSize.height());
        p.drawPixmap(rect,pixmap);
    }
}

QSize PixmapLabel::sizeHint()
{
    if (!pixmap.isNull())
    {
        QSize pixmapSize = pixmap.size();
        /*QSize totalSize = this->size();
        pixmapSize.scale(totalSize,Qt::KeepAspectRatioByExpanding);*/
        return pixmapSize;
    }
    return QSize();

}

QSize PixmapLabel::minimumSizeHint()
{
    return QSize(10,10);
}

void PixmapLabel::setPixmap(const QPixmap& pixmap)
{
    this->pixmap = pixmap;
    updateGeometry();
    repaint();
    //resizeLabel(this->size());
}

/*void PixmapLabel::resizeLabel(QSize totalSize)
{
    if (!label->pixmap())
        return;

    QSize size = label->pixmap()->size();
    size.scale(totalSize,Qt::KeepAspectRatio);

    QSize rest = (totalSize - size)/2;

    QRect rect(rest.width(),rect.height(),size.width(),size.height());

    label->setGeometry(rect);
}*/
