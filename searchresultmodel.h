#ifndef SEARCHRESULTMODEL_H
#define SEARCHRESULTMODEL_H

#include <QAbstractTableModel>

#include <QPixmap>
#include "imagestatistics.h"

class SearchResultModel : public QAbstractTableModel
{
public:
    enum Columns {
        Preview = 0,
        Path,
        Id,
        Colors,
        Size
    };

   SearchResultModel(const ImageStatisticsList& files, QObject* parent = 0);
   ~SearchResultModel();

   int rowCount(const QModelIndex &parent = QModelIndex()) const;
   int columnCount(const QModelIndex &parent = QModelIndex()) const;
   Qt::ItemFlags flags(const QModelIndex &index) const;

   QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

   static QList<QVariant> colorListToVariantList(const QList<QColor>& colors);
   static QList<QColor> variantListToColorList(const QList<QVariant>& colors);

protected:

   QList<QPixmap> m_preview;
   QList<QString> m_paths;
   QList<int> m_ids;
   QList<QList<QVariant> > m_colors;

};

#endif //SEARCHRESULTMODEL_H
