#include "opendatabasedialog.h"
#include "ui_opendatabasedialog.h"

#include "sqlquery.h"

/*static*/ int OpenDatabaseDialog::m_driverIndex = 1;

OpenDatabaseDialog::OpenDatabaseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenDatabaseDialog)
{
    ui->setupUi(this);

    ui->driver->addItem("Mysql","QMYSQL");
    ui->driver->addItem("Sqlite","QSQLITE");
    ui->driver->setCurrentIndex(m_driverIndex);
}

OpenDatabaseDialog::~OpenDatabaseDialog()
{
    delete ui;
}

DatabaseSettings OpenDatabaseDialog::settings()
{
    int index = ui->driver->currentIndex();
    QString driver = ui->driver->itemData(index).toString();
    QString hostName = ui->hostName->text();
    QString dbName = ui->dbName->text();
    QString dbUser = ui->dbUser->text();
    QString dbPass = ui->dbPass->text();
    return DatabaseSettings(driver,hostName,dbName,dbUser,dbPass);
}

void OpenDatabaseDialog::setSettings(const DatabaseSettings& settings)
{
    QComboBox* driver = ui->driver;

    int n = driver->count();
    for (int i=0;i<n;i++)
        if (driver->itemData(i).toString() == settings.driver)
            driver->setCurrentIndex(i);
    ui->hostName->setText(settings.hostName);
    ui->dbName->setText(settings.dbName);
    ui->dbUser->setText(settings.dbUser);
    ui->dbPass->setText(settings.dbPass);
}

void OpenDatabaseDialog::on_driver_currentIndexChanged(int index)
{
    m_driverIndex = index;
    setSettings(DatabaseSettings::defaults(ui->driver->itemData(index).toString()));
}
