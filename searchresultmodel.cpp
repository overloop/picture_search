#include "searchresultmodel.h"

#include <QDir>

SearchResultModel::SearchResultModel(const QList<QPair<QString, QString> > &data, const QString previewDir, QObject* parent /*= 0*/) : QAbstractTableModel(parent)
{
    QPair<QString,QString> item;
    foreach(item,data)
    {
        m_pixmaps.append(QPixmap(QDir(previewDir).filePath(item.second)));
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
