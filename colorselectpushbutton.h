#ifndef COLORSELECTPUSHBUTTON_H
#define COLORSELECTPUSHBUTTON_H

#include "colorpushbutton.h"

class ColorSelectPushButton : public ColorPushButton
{
    Q_OBJECT
public:
    explicit ColorSelectPushButton(QWidget *parent = 0);
    
signals:
    void colorSelected(QColor);
public slots:
    
private slots:
    void on_clicked();

};

#endif // COLORSELECTPUSHBUTTON_H
