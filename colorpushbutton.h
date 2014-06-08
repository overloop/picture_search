#ifndef COLORPUSHBUTTON_H
#define COLORPUSHBUTTON_H

#include <QWidget>
#include <QAbstractButton>

class ColorPushButton : public QAbstractButton
{
    Q_OBJECT
public:
    explicit ColorPushButton(QWidget *parent = 0);

    void paintEvent(QPaintEvent *event);
    QColor color() const;
    void setColor(const QColor& color);

protected:
    QColor m_color;

signals:
    
public slots:
    
};

#endif // COLORPUSHBUTTON_H
