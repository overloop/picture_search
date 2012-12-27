#ifndef OPENDATABASEDIALOG_H
#define OPENDATABASEDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include "databasesettings.h"

namespace Ui {
class OpenDatabaseDialog;
}

class OpenDatabaseDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit OpenDatabaseDialog(QWidget *parent = 0);
    ~OpenDatabaseDialog();

    DatabaseSettings settings();
    void setSettings(const DatabaseSettings &settings);

private slots:
    void on_driver_currentIndexChanged(int index);
    
private:
    Ui::OpenDatabaseDialog *ui;
    static int m_driverIndex;
};



#endif // OPENDATABASEDIALOG_H
