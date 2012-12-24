#include "colorselectpushbutton.h"

#include <QColorDialog>

ColorSelectPushButton::ColorSelectPushButton(QWidget *parent) :
    ColorPushButton(parent)
{
    connect(this,SIGNAL(clicked()),this,SLOT(on_clicked()));
}

void ColorSelectPushButton::on_clicked()
{
    QColor color = QColorDialog::getColor( m_color );
    if (color.isValid() && color != m_color)
    {
        m_color = color;
        emit colorSelected(color);
        repaint();
    }
}
