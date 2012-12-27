#ifndef PIXMAPLABEL_H
#define PIXMAPLABEL_H

#include <QWidget>

#include <QLabel>

class PixmapLabel : public QFrame
{
    Q_OBJECT
public:
    explicit PixmapLabel(QWidget *parent = 0);

    void setPixmap(const QPixmap& pixmap);
    void setText(const QString& text) {Q_UNUSED(text);}

    void paintEvent(QPaintEvent *);

    QSize sizeHint();
    QSize minimumSizeHint();

protected:
    /*QLabel* label;

    void resizeLabel(QSize totalSize);*/

    QPixmap pixmap;

signals:
    
public slots:
    
};

#endif // PIXMAPLABEL_H
