#ifndef DIRECTORIESMODEL_H
#define DIRECTORIESMODEL_H

#include <QAbstractTableModel>

class DirectoriesModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    typedef QPair<QString,bool> Item;

   DirectoriesModel(QObject* parent = 0);
   ~DirectoriesModel();

   int rowCount(const QModelIndex &parent = QModelIndex()) const;
   int columnCount(const QModelIndex &parent = QModelIndex()) const;
   Qt::ItemFlags flags(const QModelIndex &index) const;
   QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
   QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
   bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

   bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
   bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

   static void diff(const QList<Item>& before, const QList<Item>& after, bool rescan, QList<Item>& added, QList<QString>& removed);

   QList<Item> dirs(bool userMode) const {if (userMode) return m_dirsUser; else return m_dirsAll;}

   bool mode() const {return m_userMode;}

public slots:

   void setMode(bool userMode);

protected:

   QList<Item> m_dirsUser;
   QList<Item> m_dirsAll;
   bool m_write;
   bool m_userMode;
};

#endif //DIRECTORIESMODEL_H
