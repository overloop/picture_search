#include "pixmaplabel.h"

#include <QResizeEvent>
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>

PixmapLabel::PixmapLabel(QWidget *parent) :
    QFrame(parent)
{
    //label = new QLabel(this);
    //label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    //label->setScaledContents(true);
    //this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
}


void PixmapLabel::paintEvent(QPaintEvent * event)
{
    QPainter p(this);

    //QPen foregorund(palette().color(QPalette::Foreground));
    QBrush background(palette().color(QPalette::Base));

    p.fillRect(event->rect(),background);

    if (!pixmap.isNull())
    {
        QSize pixmapSize = pixmap.size();
        QSize totalSize = this->size();
        pixmapSize.scale(totalSize,Qt::KeepAspectRatio);
        QSize rest = (totalSize - pixmapSize) / 2;
        QRect rect(rest.width(),rest.height(),pixmapSize.width(),pixmapSize.height());
        p.drawPixmap(rect,pixmap);
    }

    /*p.setPen(foregorund);
    p.drawRect(QRect(QPoint(0,0),this->size()-QSize(1,1)));*/

    drawFrame(&p);

    //QFrame::paintEvent(event);
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
