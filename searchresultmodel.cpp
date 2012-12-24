#include "searchresultmodel.h"

SearchResultModel::SearchResultModel(QList<QPair<QString, QImage> > &data, QObject* parent /*= 0*/) : QAbstractTableModel(parent)
{
    QPair<QString,QImage> item;
    foreach(item,data)
    {
        m_pixmaps.append(QPixmap::fromImage(item.second));
        m_paths.append(item.first);
    }
}

SearchResultModel::~SearchResultModel()
{

}

int SearchResultModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
    return m_pixmaps.size();
}

int SearchResultModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
    return 2;
}

Qt::ItemFlags SearchResultModel::flags(const QModelIndex &index) const
{
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant SearchResultModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

QVariant SearchResultModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int r = index.row();
    int c = index.column();

    if (role == Qt::DecorationRole)
    {
        if (c==0 && r<m_pixmaps.size())
            return m_pixmaps.at(r);
    }
    if (role == Qt::DisplayRole)
    {
        if (c==1 && r<m_paths.size())
            return m_paths.at(r);
    }

	return QVariant();
}

bool SearchResultModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
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
