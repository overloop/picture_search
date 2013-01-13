#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>

#include <QDebug>
#include <QSqlError>
#include <QTimer>

#include "searchresultmodel.h"
#include "directoriesdialog.h"
#include "directoriesmodel.h"
#include "opendatabasedialog.h"
#include "database.h"
#include "settingsmodel.h"
#include "about.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_settingsModel(new SettingsModel(this))
{
    ui->setupUi(this);
    connect(&m_indexThread,SIGNAL(progress(int)),ui->progress,SLOT(setValue(int)));
    connect(&m_searchThread,SIGNAL(found()),this,SLOT(found()));
    connect(&m_indexThread,SIGNAL(indexStoped()),this,SLOT(indexStoped()));

    ui->openDatabase->setShortcut(QKeySequence::Open);
    ui->exit->setShortcut(QKeySequence::Quit);

    ui->statusbar->addWidget(ui->progress,1);

    int half = this->size().width()/2;
    ui->splitter->setSizes( QList<int>() << half << half );

    ui->searchOptions->setEnabled(false);
    ui->selectDirectories->setEnabled(false);

    SettingsModel* model = static_cast<SettingsModel*>(m_settingsModel);
    if (!model->error().isEmpty())
    {
        QMessageBox::critical(this,"Error",model->error());
        return ;
    }

    QTimer::singleShot(0,this,SLOT(on_openDatabase_triggered()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::found()
{
    QAbstractItemModel* previous = ui->previews->model();
    QList<SearchThread::SearchResult> result = m_searchThread.result();
    SearchResultModel* model = new SearchResultModel(result);
    ui->previews->setModel(model);
    connect(ui->previews->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(currentImageChanged(QModelIndex,QModelIndex)));
    delete previous;
}

void MainWindow::on_selectDirectories_triggered()
{
    DirectoriesDialog dialog;

    DirectoriesModel* model = static_cast<DirectoriesModel*>(dialog.model());

    QList<DirectoriesModel::Item> beforeUser = model->dirs(true);
    QList<DirectoriesModel::Item> beforeAll = model->dirs(false);

    if (dialog.exec() == QDialog::Accepted)
    {
        bool userMode = model->mode();

        QList<DirectoriesModel::Item> after = model->dirs(userMode);
        QList<DirectoriesModel::Item> toAdd;
        QList<QString> toRemove;
        bool rescan = dialog.rescan();

        if (userMode)
            DirectoriesModel::diff(beforeUser,after,rescan,toAdd,toRemove);
        else
            DirectoriesModel::diff(beforeAll,after,rescan,toAdd,toRemove);

        if (toAdd.size()>0)
            m_indexThread.addDirs(toAdd);
        if (toRemove.size()>0)
            m_indexThread.removeDirs(toRemove);

        m_time.start();
    }
}

void MainWindow::on_color_colorSelected(QColor color)
{
    m_searchThread.search(color,ui->deviation->value());
}

void MainWindow::on_deviation_valueChanged(int value)
{
    m_searchThread.search(ui->color->color(),value);
}

void MainWindow::indexStoped()
{
    ui->statusbar->showMessage(QString("Operation finished in %1s").arg(m_time.elapsed() / 1000));
}

void MainWindow::currentImageChanged(QModelIndex current,QModelIndex)
{
    QString path = current.model()->index(current.row(),1).data().toString();

    if (path.isEmpty())
        return;

    setWindowTitle(QString("Picture Search - ") + QDir::toNativeSeparators(path));

    QImage image(path);
    ui->image->setPixmap(QPixmap::fromImage(image));
}


void MainWindow::on_openDatabase_triggered()
{
    OpenDatabaseDialog dialog(m_settingsModel);

    ui->searchOptions->setEnabled(false);
    ui->selectDirectories->setEnabled(false);

    if (dialog.exec() == QDialog::Accepted)
    {
        DatabaseSettings settings = dialog.settings();
        QSqlDatabase db = Database::open(settings);
        if (!db.isOpen())
        {
            QMessageBox::critical(this,"Error",db.lastError().text());
            return ;
        }

        if (!Database::tablesExist())
            Database::createTables(settings.at(DatabaseSettings::DRIVER));

        ui->searchOptions->setEnabled(true);
        ui->selectDirectories->setEnabled(true);

        on_color_colorSelected(ui->color->color());
    }

}

void MainWindow::on_about_triggered()
{
    About* about = new About();
    about->setWindowFlags( about->windowFlags() | Qt::Tool );
    about->setWindowModality( Qt::ApplicationModal );
    about->setAttribute(Qt::WA_DeleteOnClose);
    about->show();
    about->activateWindow();
}
