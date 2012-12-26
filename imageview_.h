#ifndef PICTUREVIEW_H
#define PICTUREVIEW_H

#include <QTableView>
#include "imageproxymodel.h"

class ImageView : public QTableView
{
    Q_OBJECT
public:

    static const int PICTURE_SIZE = 150;

    explicit ImageView(QWidget *parent = 0);
    
    void resizeEvent(QResizeEvent *event);

    int columns() const {return m_columns;}

    void setModel(QAbstractItemModel *model);

protected:
    int m_columns;
    ImageProxyModel* proxyModel;

signals:
    
    void columnsValueChanged(int);

public slots:
    
};

#endif // PICTUREVIEW_H
