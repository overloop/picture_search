#include "pixmaplabel.h"

#include <QResizeEvent>
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>

PixmapLabel::PixmapLabel(QWidget *parent) :
    QFrame(parent)
{
    setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
}


void PixmapLabel::paintEvent(QPaintEvent * event)
{
    QPainter p(this);

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

    drawFrame(&p);
}

QSize PixmapLabel::sizeHint()
{
    if (!pixmap.isNull())
    {
        QSize pixmapSize = pixmap.size();
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

}
