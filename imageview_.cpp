#include "imageview.h"

#include <QResizeEvent>

#include <QHeaderView>

#if 0

#include "imageproxymodel.h"

ImageView::ImageView(QWidget *parent) :
    QTableView(parent)
{
    horizontalHeader()->setDefaultSectionSize(PICTURE_SIZE);
    verticalHeader()->setDefaultSectionSize(PICTURE_SIZE);
    proxyModel = new ImageProxyModel(this);
    QTableView::setModel(proxyModel);
}

void ImageView::resizeEvent(QResizeEvent *event)
{
    int newColumns = width() / PICTURE_SIZE;
    if (newColumns == 0)
        newColumns = 1;

    if (newColumns != m_columns)
    {
        m_columns = newColumns;
        //ImageProxyModel* proxyModel = static_cast<ImageProxyModel*>(model());
        proxyModel->setColumns(newColumns);
    }
}

void ImageView::setModel(QAbstractItemModel *model)
{
    //ImageProxyModel* proxyModel = static_cast<ImageProxyModel*>(this->model());
    proxyModel->setSourceModel(model);
    QTableView::setModel(proxyModel);
}

#endif
