#include "colorswidget.h"

#include <QPaintEvent>
#include <QRect>
#include <QPainter>
#include <QMouseEvent>

ColorsWidget::ColorsWidget(QWidget *parent) :
    QWidget(parent)
{

}

QSize ColorsWidget::sizeHint() const {
    return QSize(CELL_SIZE,CELL_SIZE);
}

void ColorsWidget::paintEvent(QPaintEvent* event) {

    if (m_colors.size()<1)
        return ;

    QPainter p(this);
    for (int i=0;i<m_colors.size();i++) {
        QRect rect(CELL_PADDING, CELL_SIZE*i + CELL_PADDING, CELL_SIZE - CELL_PADDING*2, CELL_SIZE - CELL_PADDING*2);
        p.fillRect(rect,QBrush(m_colors.at(i)));
    }
}

void ColorsWidget::setColors(const QList<QColor>& colors) {
    m_colors = colors;
    repaint();
}

void ColorsWidget::mousePressEvent(QMouseEvent* event) {
    QWidget::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        QColor color = colorAt(event->pos());
        if (color.isValid())
            emit clicked(color);
    }
}

QColor ColorsWidget::colorAt(const QPoint& point) const {
    int index = point.y() / CELL_SIZE;
    return m_colors.value(index);
}
