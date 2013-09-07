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
#include "taskbarprogress/qtaskbarprogress.h"
#include "version.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow),
    m_settingsModel(new SettingsModel(this)),m_about(0),m_dialog(0)
{
    ui->setupUi(this);
    /*connect(&m_indexThread,SIGNAL(progress(int)),ui->progress,SLOT(setValue(int)));
    connect(&m_searchThread,SIGNAL(found()),this,SLOT(found()));
    connect(&m_indexThread,SIGNAL(indexStoped()),this,SLOT(indexStoped()));
    connect(&m_indexThread,SIGNAL(databaseOpened(QString)),this,SLOT(databaseOpened(QString)));*/

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

    ui->progress->taskbarProgress()->setParent(this);

    setWindowTitle(QString("Picture Search %1 ").arg(PICTURE_SEARCH_VERSION_STR));

    QTimer::singleShot(0,this,SLOT(on_openDatabase_triggered()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::found()
{
    SettingsModel* settings = static_cast<SettingsModel*>(m_settingsModel);
    if (!settings)
        return ;

    QAbstractItemModel* previous = ui->previews->model();
    /*QList<SearchThread::SearchResult> result = m_searchThread.result();
    SearchResultModel* model = new SearchResultModel(result,settings->previewDir());
    ui->previews->setModel(model);
    connect(ui->previews->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(currentImageChanged(QModelIndex,QModelIndex)));*/
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

        /*if (toAdd.size()>0)
            m_indexThread.addDirs(toAdd);
        if (toRemove.size()>0)
            m_indexThread.removeDirs(toRemove);*/

    }
}

void MainWindow::on_color_colorSelected(QColor color)
{
    //m_searchThread.search(color,ui->deviation->value());
}

#include <QTimer>

void MainWindow::on_deviation_valueChanged(int value)
{
    //m_searchThread.search(ui->color->color(),value);
    qDebug() << value;


}

void MainWindow::indexStoped()
{
    //ui->statusbar->showMessage(QString("Operation completed in %1s").arg(m_time.elapsed() / 1000));

}

void MainWindow::currentImageChanged(QModelIndex current,QModelIndex)
{
    QString path = current.model()->index(current.row(),1).data().toString();

    if (path.isEmpty())
        return;

    setWindowTitle(QString("Picture Search %1 - %2 ").arg(PICTURE_SEARCH_VERSION_STR).arg(QDir::toNativeSeparators(path)));

    QImage image(path);
    ui->image->setPixmap(QPixmap::fromImage(image));
}


void MainWindow::on_openDatabase_triggered()
{
    if (!m_dialog) {
        m_dialog = new OpenDatabaseDialog(m_settingsModel,this);
    }

    ui->searchOptions->setEnabled(false);
    ui->selectDirectories->setEnabled(false);

    if (m_dialog->exec() == QDialog::Accepted)
    {
        DatabaseSettings settings = static_cast<OpenDatabaseDialog*>(m_dialog)->settings();

        /*m_time.start();
        m_indexThread.openDatabase(settings);
        m_searchThread.openDatabase(settings);*/
        //Database::open(settings,"MainThread");
        databaseOpened(QString());

        on_color_colorSelected(ui->color->color());
    }

}

void MainWindow::on_about_triggered()
{
    if (!m_about) {
        m_about = new About(this);
        m_about->setWindowFlags( m_about->windowFlags() | Qt::Tool );
    }
    m_about->show();
}

void MainWindow::databaseOpened(QString error)
{
    if (error.isEmpty())
    {
        ui->searchOptions->setEnabled(true);
        ui->selectDirectories->setEnabled(true);
    }
    else
    {
        QMessageBox::critical(this,"Error",error);
    }
}

void MainWindow::on_deviation_sliderReleased() {

}
