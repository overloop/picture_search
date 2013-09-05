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
        ui->driver->addItem(settingsModel->index(i,DatabaseSettings::Driver).data().toString());

    m_widgetMapper->setModel(m_settingsModel);
    //m_widgetMapper->setOrientation(Qt::Vertical);
    m_widgetMapper->addMapping(ui->host,DatabaseSettings::Host);
    m_widgetMapper->addMapping(ui->database,DatabaseSettings::Database);
    m_widgetMapper->addMapping(ui->user,DatabaseSettings::User);
    m_widgetMapper->addMapping(ui->pass,DatabaseSettings::Pass);
    m_widgetMapper->addMapping(ui->previewDir,DatabaseSettings::PreviewDir);

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
    settings[DatabaseSettings::Driver] = m_settingsModel->index(row,DatabaseSettings::Driver).data().toString();
    settings[DatabaseSettings::Host] = m_settingsModel->index(row,DatabaseSettings::Host).data().toString();
    settings[DatabaseSettings::Database] = m_settingsModel->index(row,DatabaseSettings::Database).data().toString();
    settings[DatabaseSettings::User] = m_settingsModel->index(row,DatabaseSettings::User).data().toString();
    settings[DatabaseSettings::Pass] = m_settingsModel->index(row,DatabaseSettings::Pass).data().toString();
    settings[DatabaseSettings::PreviewDir] = m_settingsModel->index(row,DatabaseSettings::PreviewDir).data().toString();

    return settings;
}

void OpenDatabaseDialog::on_driver_currentIndexChanged(int index)
{
    m_widgetMapper->setCurrentIndex(index);
    SettingsModel* model = static_cast<SettingsModel*>(m_settingsModel);
    model->setDriver(index);
}
