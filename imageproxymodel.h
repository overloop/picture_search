#ifndef IMAGEPROXYMODEL_H
#define IMAGEPROXYMODEL_H

#include <QAbstractProxyModel>

class ImageProxyModel : public QAbstractProxyModel
{
    Q_OBJECT
public:
    explicit ImageProxyModel(QObject *parent = 0);
    
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

    QModelIndex parent(const QModelIndex &child) const {return QModelIndex();}

    //bool insertColumns(int column, int count, const QModelIndex &parent);
    //bool insertRows(int row, int count, const QModelIndex &parent);

    void resize(int cols,int rows);
    void setColumns(int cols);

protected:
    int m_columns;

signals:
    
public slots:
    
};

#endif // IMAGEPROXYMODEL_H
