#ifndef SETTINGSMODEL_H
#define SETTINGSMODEL_H

#include <QAbstractTableModel>
#include <QSqlDatabase>
#include "databasesettings.h"

class SettingsModel : public QAbstractTableModel
{
public:
   SettingsModel(QObject* parent = 0);
   ~SettingsModel();

   int rowCount(const QModelIndex &parent = QModelIndex()) const;
   int columnCount(const QModelIndex &parent = QModelIndex()) const;
   Qt::ItemFlags flags(const QModelIndex &index) const;
   QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
   QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
   bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

   int driver() const;
   void setDriver(int driver);
   QString error() const;

   QString previewDir() const;

protected:

   QList<DatabaseSettings> m_data;
   bool m_changed;
   int m_driver;
   QString m_error;

   QString m_settingsRoot;
   QString m_sqliteRoot;

};

#endif //SETTINGSMODEL_H
