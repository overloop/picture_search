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
}
