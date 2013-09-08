#ifndef PIXMAPLABEL_H
#define PIXMAPLABEL_H

#include <QWidget>

#include <QLabel>

class QMovie;

class PixmapLabel : public QLabel
{
    Q_OBJECT
public:
    explicit PixmapLabel(QWidget *parent = 0);
    ~PixmapLabel();

    void setImage(const QString &image);
    //void setPixmap(const QPixmap& m_pixmap);
    void setText(const QString& text) {Q_UNUSED(text);}

    void paintEvent(QPaintEvent *);

    QSize sizeHint();
    QSize minimumSizeHint();

protected:

    QPixmap m_pixmap;
    QMovie* m_movie;

signals:
    
public slots:
    
};

#endif // PIXMAPLABEL_H
