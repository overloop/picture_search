#include "settingsmodel.h"

#include <QSettings>
#include <QtGlobal>
#include <QDir>
#include <QApplication>
#include "databasesettings.h"
#include <stdlib.h>

SettingsModel::SettingsModel(QObject* parent /*= 0*/) : QAbstractTableModel(parent), m_changed(false), m_driver(0)
{

#ifdef Q_OS_WIN
    m_settingsRoot = QString(getenv("APPDATA"));
    m_sqliteRoot = m_settingsRoot;
#endif

#ifdef Q_OS_LINUX
    m_settingsRoot = "/usr/local/etc";
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
    QString varPath = QDir::toNativeSeparators(sqliteDir.absolutePath());

    QSettings settings(settingsDir.filePath("settings.ini"),QSettings::IniFormat);

    settings.beginGroup("QSQLITE");
    DatabaseSettings sqlite;
    sqlite[DatabaseSettings::Driver] = "QSQLITE";
    sqlite[DatabaseSettings::Host] = settings.value("host","").toString();
    sqlite[DatabaseSettings::Database] = settings.value("database",QDir::toNativeSeparators(sqliteDir.filePath("db.sqlite"))).toString();
    sqlite[DatabaseSettings::User] = settings.value("user","").toString();
    sqlite[DatabaseSettings::Pass] = settings.value("pass","").toString();
    sqlite[DatabaseSettings::PreviewDir] = settings.value("preview_dir",varPath).toString();
    settings.endGroup();

    settings.beginGroup("QMYSQL");
    DatabaseSettings mysql;
    mysql[DatabaseSettings::Driver] = "QMYSQL";
    mysql[DatabaseSettings::Host] = settings.value("host","localhost").toString();
    mysql[DatabaseSettings::Database] = settings.value("database","picture_search").toString();
    mysql[DatabaseSettings::User] = settings.value("user","root").toString();
    mysql[DatabaseSettings::Pass] = settings.value("pass","").toString();
    mysql[DatabaseSettings::PreviewDir] = settings.value("preview_dir",varPath).toString();
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
    settings.setValue("host",m_data.at(0).at(DatabaseSettings::Host));
    settings.setValue("database",m_data.at(0).at(DatabaseSettings::Database));
    settings.setValue("user",m_data.at(0).at(DatabaseSettings::User));
    settings.setValue("pass",m_data.at(0).at(DatabaseSettings::Pass));
    settings.setValue("preview_dir",m_data.at(0).at(DatabaseSettings::PreviewDir));
    settings.endGroup();

    settings.beginGroup("QMYSQL");
    settings.setValue("host",m_data.at(1).at(DatabaseSettings::Host));
    settings.setValue("database",m_data.at(1).at(DatabaseSettings::Database));
    settings.setValue("user",m_data.at(1).at(DatabaseSettings::User));
    settings.setValue("pass",m_data.at(1).at(DatabaseSettings::Pass));
    settings.setValue("preview_dir",m_data.at(1).at(DatabaseSettings::PreviewDir));
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
    return DatabaseSettings::Size;
}

Qt::ItemFlags SettingsModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
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
        if (r>=0 && r <m_data.size() && c>=0 && c<DatabaseSettings::Size)
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
        if (r>=0 && r <m_data.size() && c>=0 && c<DatabaseSettings::Size)
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

QString SettingsModel::previewDir() const {
    return data(this->index(m_driver,DatabaseSettings::PreviewDir)).toString();
}

void SettingsModel::setDriver(int driver)
{
    m_driver = driver;
    m_changed = true;
}

