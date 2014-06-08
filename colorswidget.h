#ifndef COLORSWIDGET_H
#define COLORSWIDGET_H

#include <QWidget>

class ColorsWidget : public QWidget
{
    Q_OBJECT
public:
    static const int CELL_SIZE = 32;
    static const int CELL_PADDING = 1;

    explicit ColorsWidget(QWidget *parent = 0);
    
    void setColors(const QList<QColor>& colors);

    QSize sizeHint() const;

    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *);

    QColor colorAt(const QPoint& point) const;

protected:
    QList<QColor> m_colors;

signals:
    void clicked(QColor);
    
public slots:
    
};

#endif // COLORSWIDGET_H
