#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QResizeEvent>
#include "searchresultmodel.h"
#include "directoriesdialog.h"
#include "directoriesmodel.h"
#include "opendatabasedialog.h"

#include <QImage>
#include <QPainter>
#include <QDebug>
#include <QSqlError>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&indexThread,SIGNAL(progress(int)),ui->progress,SLOT(setValue(int)));
    connect(&searchThread,SIGNAL(found()),this,SLOT(found()));
    connect(&indexThread,SIGNAL(indexStoped()),this,SLOT(indexStoped()));
    //connect(ui->table,SIGNAL(clicked(QModelIndex)),this,SLOT(pictureClicked(QModelIndex)));

    //ui->table->setEditTriggers(QAbstractItemView::CurrentChanged);
    //connect(ui->table,SIGNAL(pressed(QModelIndex))

    ui->statusbar->addWidget(ui->progress,1);

    /*QSizePolicy policy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    policy.setHeightForWidth(true);
    ui->image->setSizePolicy(policy);*/

    /*ui->browser->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->browser->setGeometry(QRect(0,0,100,100));*/

    /*QImage image(10,10,QImage::Format_RGB32);
    QPainter p(&image);
    p.fillRect(QRect(0,0,10,10),QColor(Qt::yellow));
    ui->browser->setPixmap(QPixmap::fromImage(image));*/


    int half = this->size().width()/2;
    ui->splitter->setSizes( QList<int>() << half << half );

    ui->searchOptions->setEnabled(false);
    ui->selectDirectories->setEnabled(false);

    QTimer::singleShot(0,this,SLOT(on_openDatabase_triggered()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::found()
{
    QAbstractItemModel* previous = ui->previews->model();
    QList<SearchThread::SearchResult> result = searchThread.result();
    SearchResultModel* model = new SearchResultModel(result);
    ui->previews->setModel(model);
    connect(ui->previews->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(currentImageChanged(QModelIndex,QModelIndex)));
    delete previous;
}



void MainWindow::on_selectDirectories_triggered()
{
    /*QString path = QFileDialog::getExistingDirectory(this);
    if (path.isEmpty())
        return;
    if (QMessageBox::information(this, "Subdirectories", "Add subdirectories", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        indexThread.addDir(path,true);
    } else {
        indexThread.addDir(path,false);
    }
    time.start();*/

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
            indexThread.addDirs(toAdd);
        if (toRemove.size()>0)
            indexThread.removeDirs(toRemove);

        time.start();
    }
}

void MainWindow::on_color_colorSelected(QColor color)
{
    searchThread.search(color,ui->deviation->value());
}

void MainWindow::on_deviation_valueChanged(int value)
{
    searchThread.search(ui->color->color(),value);
}

void MainWindow::indexStoped()
{
    ui->statusbar->showMessage(QString("Operation finished in %1s").arg(time.elapsed() / 1000));
}

void MainWindow::currentImageChanged(QModelIndex current,QModelIndex)
{
    QString path = current.model()->index(current.row(),1).data().toString();

    //qDebug() << path << " <- path";

    if (path.isEmpty())
        return;

    setWindowTitle(QString("Picture Search - ") + QDir::toNativeSeparators(path));

    QImage image(path);
    ui->image->setPixmap(QPixmap::fromImage(image));
}



void MainWindow::on_openDatabase_triggered()
{
    OpenDatabaseDialog dialog;
    //dialog.setSettings(DatabaseSettings::defaults("QSQLITE"));

    if (dialog.exec() == QDialog::Accepted)
    {
        DatabaseSettings settings = dialog.settings();
        QSqlDatabase db = openDb(settings);
        if (!db.isOpen())
        {
            QMessageBox::critical(this,"Error",db.lastError().text());
            return ;
        }

        if (!tablesExist())
            createTables(settings.driver);

        ui->searchOptions->setEnabled(true);
        ui->selectDirectories->setEnabled(true);

        DatabaseSettings::setDefaults(settings);
    }


}
