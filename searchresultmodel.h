#ifndef SEARCHRESULTMODEL_H
#define SEARCHRESULTMODEL_H

#include <QAbstractTableModel>

#include <QPixmap>
#include "imagestatistics.h"

class SearchResultModel : public QAbstractTableModel
{
public:
   SearchResultModel(const QList<ImageStatistics>& files, QObject* parent = 0);
   ~SearchResultModel();

   int rowCount(const QModelIndex &parent = QModelIndex()) const;
   int columnCount(const QModelIndex &parent = QModelIndex()) const;
   Qt::ItemFlags flags(const QModelIndex &index) const;

   QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
/*
   bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
   QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
   bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
   bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
*/

protected:

   QList<QPixmap> m_preview;
   QList<QString> m_paths;
   QList<int> m_ids;

};

#endif //SEARCHRESULTMODEL_H
