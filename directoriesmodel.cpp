#include "directoriesmodel.h"

#include "sqlquery.h"

DirectoriesModel::DirectoriesModel(QObject* parent /*= 0*/) :
    QAbstractTableModel(parent),
    m_write(true),
    m_userMode(true)
{
   SqlQuery q;
   q.prepare("SELECT path,subdirs,user FROM directory");
   Q_ASSERT(q.exec());
   while (q.next())
   {
       Item item(q.value(0).toString(),q.value(1).toInt()==1);
       if (q.value(2).toInt() == 1)
           m_dirsUser.append(item);
       m_dirsAll.append(item);
   }
}

DirectoriesModel::~DirectoriesModel()
{

}

int DirectoriesModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;

    if (m_userMode)
        return m_dirsUser.size();
    else
        return m_dirsAll.size();
}

int DirectoriesModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
    return 2;
}

Qt::ItemFlags DirectoriesModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    switch (index.column())
    {
    case 0:
        flags = flags | Qt::ItemIsUserCheckable;
        break;
    case 1:
        flags = flags | Qt::ItemIsEditable;
        break;
    }

    return flags;
}

QVariant DirectoriesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case 0:
            return "subdirs";
        case 1:
            return "directory";
        }
    }

	return QVariant();
}

QVariant DirectoriesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int col = index.column();
    int row = index.row();

    if (!(row>=0 && row<rowCount()))
        return QVariant();

    if (col == 0 && role == Qt::CheckStateRole)
    {
        bool isChecked;

        if (m_userMode)
            isChecked = m_dirsUser.at(row).second;
        else
            isChecked = m_dirsAll.at(row).second;

        if (isChecked)
            return Qt::Checked;
        else
            return Qt::Unchecked;
    }

    if (col == 1 && (role == Qt::DisplayRole || role == Qt::EditRole))
    {
        if (m_userMode)
            return m_dirsUser.at(row).first;
        else
            return m_dirsAll.at(row).first;
    }


	return QVariant();
}

bool DirectoriesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    int col = index.column();
    int row = index.row();

    if (col == 0 && role == Qt::CheckStateRole)
    {
        bool isChecked;

        if (static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked)
            isChecked = true;
        else
            isChecked = false;

        if (m_userMode)
            m_dirsUser[row] = Item(m_dirsUser.at(row).first,isChecked);
        else
            m_dirsAll[row] = Item(m_dirsAll.at(row).first,isChecked);

        emit dataChanged(index,index);
        return true;
    }

    if (col == 1 && role == Qt::EditRole)
    {

        if (m_userMode)
            m_dirsUser[row] = Item(value.toString(),m_dirsUser.at(row).second);
        else
            m_dirsAll[row] = Item(value.toString(),m_dirsAll.at(row).second);

        emit dataChanged(index,index);
        return true;
    }

	return false;
}

bool DirectoriesModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid())
        return false;

    beginInsertRows(parent,row,row+count-1);

    if (m_write)
        if (m_userMode)
            for (int i=0;i<count;i++)
                m_dirsUser.insert(row,Item());
        else
            for (int i=0;i<count;i++)
                m_dirsAll.insert(row,Item());

    endInsertRows();

}

bool DirectoriesModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid())
        return false;

    beginRemoveRows(parent,row,row+count-1);

    if (m_write)
        if (m_userMode)
            for (int i=0;i<count;i++)
                m_dirsUser.removeAt(row);
        else
            for (int i=0;i<count;i++)
                m_dirsAll.removeAt(row);

    endRemoveRows();

	return false;
}

/*static*/
void DirectoriesModel::diff(const QList<Item>& before, const QList<Item> &after, bool rescan, QList<Item>& added, QList<QString>& removed)
{
    QHash<QString,bool> beforeHash;
    QHash<QString,bool> afterHash;

    Item item;
    foreach(item,before)
        beforeHash.insert(item.first,item.second);
    foreach(item,after)
        afterHash.insert(item.first,item.second);

    foreach(item,after)
    {
        if (beforeHash.contains(item.first))
        {
            if (rescan)
                added.append(item);
        }
        else
        {
            added.append(item);
        }
    }

    foreach(item,before)
    {
        if (!afterHash.contains(item.first))
            removed.append(item.first);
    }
}

void DirectoriesModel::setMode(bool userMode)
{
    if (m_userMode != userMode)
    {
        m_write = false;

        int rows = rowCount();
        if (rows>0)
            removeRows(0,rows);

        m_userMode = userMode;

        rows = rowCount();

        if (rows>0)
            insertRows(0,rows);

        m_write = true;
    }
}


