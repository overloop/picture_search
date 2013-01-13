#include "opendatabasedialog.h"
#include "ui_opendatabasedialog.h"

#include "databasesettings.h"
#include <QDataWidgetMapper>
#include "settingsmodel.h"

OpenDatabaseDialog::OpenDatabaseDialog(QAbstractItemModel *settingsModel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenDatabaseDialog),
    m_settingsModel(settingsModel),
    m_widgetMapper(new QDataWidgetMapper(this))
{
    ui->setupUi(this);

    for (int i=0;i<settingsModel->rowCount();i++)
        ui->driver->addItem(settingsModel->index(i,DatabaseSettings::DRIVER).data().toString());

    m_widgetMapper->setModel(m_settingsModel);
    //m_widgetMapper->setOrientation(Qt::Vertical);
    m_widgetMapper->addMapping(ui->host,DatabaseSettings::HOST);
    m_widgetMapper->addMapping(ui->database,DatabaseSettings::DATABASE);
    m_widgetMapper->addMapping(ui->user,DatabaseSettings::USER);
    m_widgetMapper->addMapping(ui->pass,DatabaseSettings::PASS);

    SettingsModel* model = static_cast<SettingsModel*>(m_settingsModel);
    m_widgetMapper->setCurrentIndex( model->driver() );
}

OpenDatabaseDialog::~OpenDatabaseDialog()
{
    delete ui;
}

DatabaseSettings OpenDatabaseDialog::settings()
{
    SettingsModel* model = static_cast<SettingsModel*>(m_settingsModel);
    int row = model->driver();

    DatabaseSettings settings;
    settings[DatabaseSettings::DRIVER] = m_settingsModel->index(row,DatabaseSettings::DRIVER).data().toString();
    settings[DatabaseSettings::HOST] = m_settingsModel->index(row,DatabaseSettings::HOST).data().toString();
    settings[DatabaseSettings::DATABASE] = m_settingsModel->index(row,DatabaseSettings::DATABASE).data().toString();
    settings[DatabaseSettings::USER] = m_settingsModel->index(row,DatabaseSettings::USER).data().toString();
    settings[DatabaseSettings::PASS] = m_settingsModel->index(row,DatabaseSettings::PASS).data().toString();

    return settings;
}

void OpenDatabaseDialog::on_driver_currentIndexChanged(int index)
{
    m_widgetMapper->setCurrentIndex(index);
    SettingsModel* model = static_cast<SettingsModel*>(m_settingsModel);
    model->setDriver(index);
}
