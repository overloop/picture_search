#include "imageproxymodel.h"

ImageProxyModel::ImageProxyModel(QObject *parent) :
    QAbstractProxyModel(parent),
    m_columns(0)
{

}

QModelIndex ImageProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    if (!sourceIndex.isValid())
        return QModelIndex();

    int row = sourceIndex.row();
    int r = row / m_columns;
    int c = row % m_columns;
    return index(r,c);
}

QModelIndex ImageProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if (!proxyIndex.isValid())
        return QModelIndex();

    int r = proxyIndex.row();
    int c = proxyIndex.column();
    return sourceModel()->index(r*m_columns+c,0);
}

QModelIndex ImageProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return createIndex(row,column);
}

int ImageProxyModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    if (!sourceModel())
        return 0;
    int rows = sourceModel()->rowCount();
    if ((rows % m_columns) == 0)
        return rows / m_columns;
    else
        return rows / m_columns + 1;
}

int ImageProxyModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_columns;
}

void ImageProxyModel::resize(int cols,int rows)
{
    int r = rowCount();
    int c = columnCount();

    if (r>rows)
    {
        beginRemoveRows(QModelIndex(),rows,r-1);
        endRemoveRows();
    }
    if (c>cols)
    {
        beginRemoveColumns(QModelIndex(),cols,c-1);
        endRemoveColumns();
    }
    if (r<rows)
    {
        beginInsertRows(QModelIndex(),r,rows-1);
        endInsertRows();
    }
    if (c<cols)
    {
        beginInsertColumns(QModelIndex(),c,cols-1);
        endInsertColumns();
    }
}

void ImageProxyModel::setColumns(int cols)
{
    if (!sourceModel())
        return;
    int rows = sourceModel()->rowCount();
    int newRows;
    if ((rows % cols) == 0)
        newRows = rows / cols;
    else
        newRows = rows / cols + 1;
    resize(newRows,cols);
    m_columns = cols;
}
