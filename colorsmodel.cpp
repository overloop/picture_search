#include "colorsmodel.h"

#include <QSize>

ColorsModel::ColorsModel(const QList<QColor> &colors, QObject *parent) :
    QAbstractTableModel(parent), m_colors(colors)
{

}

QVariant ColorsModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::SizeHintRole) {
        return QSize(0,0);
    }

    return QAbstractItemModel::headerData(section,orientation,role);
}

QVariant ColorsModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role == Qt::BackgroundRole) {
        if (index.column() == 0)
            return m_colors.value(index.row());
    }

    if (role == Qt::SizeHintRole) {
        if (index.column() == 0)
            return QSize(30,30);
    }

    return QVariant();
}

int ColorsModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return m_colors.size();
}

int ColorsModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return 1;
}
