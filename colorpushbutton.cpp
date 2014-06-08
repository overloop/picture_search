#include "colorpushbutton.h"

#include <QPainter>
#include <QPaintEvent>

ColorPushButton::ColorPushButton(QWidget *parent) :
    QAbstractButton(parent),
    m_color(Qt::white)
{
    setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFixedSize(20,20);
}

void ColorPushButton::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    p.fillRect(event->rect(), m_color);

    QPen pen(palette().color(QPalette::Foreground));
    p.setPen(pen);
    p.drawRect(QRect(QPoint(0,0),this->size()-QSize(1,1)));
}

QColor ColorPushButton::color() const {
    return m_color;
}

void ColorPushButton::setColor(const QColor& color) {
    m_color = color;
    repaint();
}
