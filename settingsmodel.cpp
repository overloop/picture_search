#include "settingsmodel.h"

#include <QSettings>
#include <QtGlobal>
#include <QDir>
#include <QApplication>
#include "databasesettings.h"

SettingsModel::SettingsModel(QObject* parent /*= 0*/) : QAbstractTableModel(parent), m_changed(false), m_driver(0)
{

#ifdef Q_OS_WIN
    m_settingsRoot = QString(getenv("APPDATA"));
    m_sqliteRoot = m_settingsRoot;
#endif

#ifdef Q_OS_LINUX
    m_settingsRoot = "/usr/etc";
    m_sqliteRoot = "/var";
#endif

    if (m_settingsRoot.isEmpty())
    {
        QDir dir(qApp->applicationDirPath());
        dir.cd("..");
        m_settingsRoot = dir.path();
        m_sqliteRoot = dir.path();
    }

    QDir settingsDir(m_settingsRoot);
    if (!settingsDir.cd("picture_search"))
    {
        m_error = QString("Directory %1 doesn't exist").arg(settingsDir.filePath("picture_search"));
        return ;
    }

    QDir sqliteDir(m_sqliteRoot);
    if (!sqliteDir.cd("picture_search"))
    {
        m_error = QString("Directory %1 doesn't exist").arg(settingsDir.filePath("picture_search"));
        return ;
    }

    QSettings settings(settingsDir.filePath("settings.ini"),QSettings::IniFormat);

    settings.beginGroup("QSQLITE");
    DatabaseSettings sqlite;
    sqlite[DatabaseSettings::DRIVER] = "QSQLITE";
    sqlite[DatabaseSettings::HOST] = settings.value("host","").toString();
    sqlite[DatabaseSettings::DATABASE] = settings.value("database",sqliteDir.filePath("db.sqlite")).toString();
    sqlite[DatabaseSettings::USER] = settings.value("user","").toString();
    sqlite[DatabaseSettings::PASS] = settings.value("pass","").toString();
    settings.endGroup();

    settings.beginGroup("QMYSQL");
    DatabaseSettings mysql;
    mysql[DatabaseSettings::DRIVER] = "QMYSQL";
    mysql[DatabaseSettings::HOST] = settings.value("host","localhost").toString();
    mysql[DatabaseSettings::DATABASE] = settings.value("database","picture_search").toString();
    mysql[DatabaseSettings::USER] = settings.value("user","root").toString();
    mysql[DatabaseSettings::PASS] = settings.value("pass","").toString();
    settings.endGroup();

    settings.beginGroup("DRIVER");
    m_driver = settings.value("index",0).toInt();
    settings.endGroup();

    m_data.append(sqlite);
    m_data.append(mysql);
}

SettingsModel::~SettingsModel()
{
    if (!m_changed)
        return;

    QDir settingsDir(m_settingsRoot);
    settingsDir.cd("picture_search");
    QSettings settings(settingsDir.filePath("settings.ini"),QSettings::IniFormat);

    settings.beginGroup("QSQLITE");
    settings.setValue("host",m_data.at(0).at(DatabaseSettings::HOST));
    settings.setValue("database",m_data.at(0).at(DatabaseSettings::DATABASE));
    settings.setValue("user",m_data.at(0).at(DatabaseSettings::USER));
    settings.setValue("pass",m_data.at(0).at(DatabaseSettings::PASS));
    settings.endGroup();

    settings.beginGroup("QMYSQL");
    settings.setValue("host",m_data.at(1).at(DatabaseSettings::HOST));
    settings.setValue("database",m_data.at(1).at(DatabaseSettings::DATABASE));
    settings.setValue("user",m_data.at(1).at(DatabaseSettings::USER));
    settings.setValue("pass",m_data.at(1).at(DatabaseSettings::PASS));
    settings.endGroup();

    settings.beginGroup("DRIVER");
    settings.setValue("index",m_driver);
    settings.endGroup();
}

int SettingsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_data.size();
}

int SettingsModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return DatabaseSettings::SIZE;
}

Qt::ItemFlags SettingsModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

QVariant SettingsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section) Q_UNUSED(orientation) Q_UNUSED(role)
    return QVariant();
}

QVariant SettingsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        int r = index.row();
        int c = index.column();
        if (r>=0 && r <m_data.size() && c>=0 && c<DatabaseSettings::SIZE)
            return m_data.at(r).at(c);
    }

    return QVariant();
}

bool SettingsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    if (role == Qt::EditRole)
    {
        int r = index.row();
        int c = index.column();
        if (r>=0 && r <m_data.size() && c>=0 && c<DatabaseSettings::SIZE)
        {
            m_data[r][c] = value.toString();
            emit dataChanged(index,index);
            m_changed = true;
            return true;
        }
    }

    return false;
}


QString SettingsModel::error() const
{
    return m_error;
}

int SettingsModel::driver() const
{
    return m_driver;
}

void SettingsModel::setDriver(int driver)
{
    m_driver = driver;
    m_changed = true;
}

