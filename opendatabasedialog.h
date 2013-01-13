#ifndef OPENDATABASEDIALOG_H
#define OPENDATABASEDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include "databasesettings.h"
class QAbstractItemModel;
class QDataWidgetMapper;

namespace Ui {
class OpenDatabaseDialog;
}

class OpenDatabaseDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit OpenDatabaseDialog(QAbstractItemModel* settingsModel, QWidget *parent = 0);
    ~OpenDatabaseDialog();

    DatabaseSettings settings();

private slots:
    void on_driver_currentIndexChanged(int index);
    
private:
    Ui::OpenDatabaseDialog *ui;
    QAbstractItemModel* m_settingsModel;
    QDataWidgetMapper* m_widgetMapper;
};



#endif // OPENDATABASEDIALOG_H
