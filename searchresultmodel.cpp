#include "searchresultmodel.h"

#include <QDir>

SearchResultModel::SearchResultModel(const QList<ImageStatistics> &files, QObject* parent /*= 0*/) : QAbstractTableModel(parent)
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

#if 0

bool SearchResultModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index)
    Q_UNUSED(value)
    Q_UNUSED(role)

	return false;
}

bool SearchResultModel::insertRows(int row, int count, const QModelIndex &parent)
{
	return false;
}

bool SearchResultModel::removeRows(int row, int count, const QModelIndex &parent)
{
	return false;
}

QVariant SearchResultModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(role)
    return QVariant();
}

#endif
