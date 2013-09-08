#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>

#include <QDebug>
#include <QSqlError>
#include <QTimer>

#include "searchresultmodel.h"
#include "directoriesdialog.h"

#include "opendatabasedialog.h"

#include "settingsmodel.h"
#include "about.h"
#include "taskbarprogress/qtaskbarprogress.h"
#include "picture_search.h"

#include "databaseworker.h"
#include "indexworker.h"

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

    /*QStringList dirs;
    dirs << "one" << "two" << "three";
    DirectoriesDialog dialog(dirs);
    dialog.exec();
    qDebug() << dialog.directoriesToAdd();
    qDebug() << dialog.directoriesToRemove();*/

    IndexWorker* indexWorker = new IndexWorker();
    DatabaseWorker* databaseWorker = new DatabaseWorker();

    indexWorker->moveToThread(&indexThread);
    databaseWorker->moveToThread(&databaseThread);

    // open database
    connect(this,SIGNAL(openDatabase(QStringList)),indexWorker,SLOT(openDatabase(QStringList)));
    connect(this,SIGNAL(openDatabase(QStringList)),databaseWorker,SLOT(openDatabase(QStringList)));
    connect(databaseWorker,SIGNAL(databaseOpened(bool,QString)),this,SLOT(databaseOpened(bool,QString)));

    // build index
    connect(ui->selectDirectories,SIGNAL(triggered()),databaseWorker,SLOT(selectDirectories()));
    connect(databaseWorker,SIGNAL(directoriesSelected(QStringList)),this,SLOT(directoriesSelected(QStringList)));
    connect(this,SIGNAL(scanDirectories(QStringList,QStringList)),databaseWorker,SLOT(scanDirectories(QStringList,QStringList)));
    connect(this,SIGNAL(scanDirectories(QStringList,QStringList)),indexWorker,SLOT(scanDirectories(QStringList,QStringList)));
    connect(indexWorker,SIGNAL(filesScaned(QStringList)),databaseWorker,SLOT(filesScaned(QStringList)));
    connect(databaseWorker,SIGNAL(filesUnindexed(QStringList)),indexWorker,SLOT(filesUnindexed(QStringList)));
    connect(indexWorker,SIGNAL(filesAnalyzed(ImageStatisticsList,bool,QTime)),databaseWorker,SLOT(filesAnalyzed(ImageStatisticsList,bool,QTime)));

    // find in index
    connect(this,SIGNAL(findFiles(QColor,int)),databaseWorker,SLOT(findFiles(QColor,int)));
    connect(databaseWorker,SIGNAL(filesFound(ImageStatisticsList)),this,SLOT(filesFound(ImageStatisticsList)));

    // reporting
    connect(databaseWorker,SIGNAL(error(QString)),this,SLOT(error(QString)));
    connect(indexWorker,SIGNAL(progress(int)),ui->progress,SLOT(setValue(int)));
    connect(databaseWorker,SIGNAL(status(QString)),this,SLOT(status(QString)));
    connect(indexWorker,SIGNAL(status(QString)),this,SLOT(status(QString)));


    // cleanup
    connect(&indexThread,SIGNAL(finished()),indexWorker,SLOT(deleteLater()));
    connect(&databaseThread,SIGNAL(finished()),databaseWorker,SLOT(deleteLater()));

    indexThread.start();
    databaseThread.start();

    ui->progress->setMaximum(1000);

    QTimer::singleShot(0,this,SLOT(on_openDatabase_triggered()));
}

MainWindow::~MainWindow()
{
    if (ui->previews->model())
        ui->previews->model()->deleteLater();
    if (ui->previews->selectionModel())
        ui->previews->selectionModel()->deleteLater();

    indexThread.quit();
    databaseThread.quit();
    indexThread.wait();
    databaseThread.wait();

    delete ui;
}

void MainWindow::directoriesSelected(const QStringList& dirs) {
    DirectoriesDialog dialog(dirs);
    if (dialog.exec() == QDialog::Accepted)
        emit scanDirectories(dialog.directoriesToAdd(),dialog.directoriesToRemove());
    ui->progress->reset();
}

void MainWindow::filesFound(const ImageStatisticsList &files) {
    SearchResultModel* model = new SearchResultModel(files);
    if (ui->previews->model())
        ui->previews->model()->deleteLater();
    if (ui->previews->selectionModel())
        ui->previews->selectionModel()->deleteLater();
    ui->previews->setModel(model);
    connect(ui->previews->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(currentImageChanged(QModelIndex,QModelIndex)));
}

/*
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

    }
}
*/


void MainWindow::on_color_colorSelected(QColor color) {
    emit findFiles(color,ui->deviation->value());
}

void MainWindow::on_deviation_sliderReleased() {
    emit findFiles(ui->color->color(),ui->deviation->value());
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
        QStringList settings = static_cast<OpenDatabaseDialog*>(m_dialog)->settings();
        emit openDatabase(settings);
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

void MainWindow::databaseOpened(bool ok, const QString &error)
{
    if (ok)
    {
        ui->searchOptions->setEnabled(true);
        ui->selectDirectories->setEnabled(true);
    }
    else
    {
        QMessageBox::critical(this,"Error",error);
    }
}

void MainWindow::error(const QString& text) {
    QMessageBox::critical(this,"Error",text);
}

void MainWindow::status(const QString& text) {
    ui->statusbar->showMessage(text);
}
