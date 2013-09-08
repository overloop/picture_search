#include "imagelabel.h"

#include <QResizeEvent>
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>

#include <QImageReader>
#include <QMovie>

ImageLabel::ImageLabel(QWidget *parent) :
    QLabel(parent), m_movie(0)
{
    //setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    setAlignment(Qt::AlignCenter);
}

ImageLabel::~ImageLabel() {
    if (m_movie)
        m_movie->deleteLater();
    m_movie = 0;
}

void ImageLabel::paintEvent(QPaintEvent * event)
{
    QPainter p(this);

    QBrush background(palette().color(QPalette::Base));

    p.fillRect(event->rect(),background);

    if (m_pixmap.isNull()) {
        QLabel::paintEvent(event);
    } else {
        QSize pixmapSize = m_pixmap.size();
        QSize totalSize = this->size();
        pixmapSize.scale(totalSize,Qt::KeepAspectRatio);
        QSize rest = (totalSize - pixmapSize) / 2;
        QRect rect(rest.width(),rest.height(),pixmapSize.width(),pixmapSize.height());
        p.drawPixmap(rect,m_pixmap);
    }

    drawFrame(&p);
}

QSize ImageLabel::sizeHint() {
    if (!m_pixmap.isNull())
        return  m_pixmap.size();
    if (m_movie)
        return m_movie->currentPixmap().size();
    return QSize();
}

QSize ImageLabel::minimumSizeHint() {
    return QSize(10,10);
}

void ImageLabel::setImage(const QString& image) {

    QImageReader reader(image);
    if (reader.supportsAnimation() && reader.imageCount()>1) {
        m_movie = new QMovie(image);
        this->setMovie(m_movie);
        m_movie->start();
        m_pixmap = QPixmap();
    } else {
        if (m_movie)
            m_movie->deleteLater();
        m_movie = 0;
        m_pixmap = QPixmap(image);
        updateGeometry();
        repaint();
    }
}

/*
void PixmapLabel::setPixmap(const QPixmap& pixmap)
{
    this->m_pixmap = pixmap;
    updateGeometry();
    repaint();

}*/
