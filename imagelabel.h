#ifndef PIXMAPLABEL_H
#define PIXMAPLABEL_H

#include <QWidget>

#include <QLabel>

class QMovie;

class ImageLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ImageLabel(QWidget *parent = 0);
    ~ImageLabel();

    void setImage(const QString &image);

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
