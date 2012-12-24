#ifndef SEARCHRESULTMODEL_H
#define SEARCHRESULTMODEL_H

#include <QAbstractTableModel>

#include <QPixmap>

class SearchResultModel : public QAbstractTableModel
{
public:
   SearchResultModel( QList<QPair<QString, QImage> > &data, QObject* parent = 0);
   ~SearchResultModel();

   int rowCount(const QModelIndex &parent = QModelIndex()) const;
   int columnCount(const QModelIndex &parent = QModelIndex()) const;
   Qt::ItemFlags flags(const QModelIndex &index) const;
   QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
   QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
   bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

   bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
   bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

protected:

   QList<QPixmap> m_pixmaps;
   QList<QString> m_paths;

};

#endif //SEARCHRESULTMODEL_H
