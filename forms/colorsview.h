#ifndef COLORSVIEW_H
#define COLORSVIEW_H

#include <QTableView>

class ColorsView : public QTableView
{
    Q_OBJECT
public:
    explicit ColorsView(QWidget *parent = 0);

    void setModel(QAbstractItemModel *model);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

signals:
    
public slots:
    
};

#endif // COLORSVIEW_H
