#include "searchresultmodel.h"

#include <QDir>

SearchResultModel::SearchResultModel(const ImageStatisticsList &files, QObject* parent /*= 0*/) : QAbstractTableModel(parent)
{
    ImageStatistics stat;
    foreach(stat,files)
    {
        m_ids << stat.id;
        m_paths << stat.file;
        m_preview << QPixmap(stat.preview);
    }
}

SearchResultModel::~SearchResultModel()
{

}

int SearchResultModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
    return m_preview.size();
}

int SearchResultModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
    return 3;
}

Qt::ItemFlags SearchResultModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}


QVariant SearchResultModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int r = index.row();
    int c = index.column();

    if (role == Qt::DecorationRole)
    {
        if (c==0)
            return m_preview.value(r);
    }
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if (c==1)
            return m_paths.value(r);
        if (c==2)
            return m_ids.value(r);
    }

	return QVariant();
}

