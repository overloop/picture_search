#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>

#include <QDebug>
#include <QSqlError>
#include <QTimer>
#include <QFileDialog>

#include "searchresultmodel.h"
#include "directoriesdialog.h"

#include "opendatabasedialog.h"

#include "settingsmodel.h"
#include "about.h"
#include "taskbarprogress/qtaskbarprogress.h"
#include "picture_search.h"

#include "databaseworker.h"
#include "indexworker.h"
#include "colorsmodel.h"

#include "imagedialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow),
    m_settingsModel(new SettingsModel(this)),m_about(0),m_dialog(0)
{
    ui->setupUi(this);

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

    IndexWorker* indexWorker = new IndexWorker();
    DatabaseWorker* databaseWorker = new DatabaseWorker();

    indexWorker->moveToThread(&indexThread);
    databaseWorker->moveToThread(&databaseThread);

    /** @see connections.jpg for connection diagram */

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

    //connect(ui->colors,SIGNAL())

    indexThread.start();
    databaseThread.start();

    //ui->progress->setMaximum(1000);

    //ImageDialog dialog("D:/temp/test/13780109064809.jpg");
    //dialog.exec();

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

void MainWindow::on_colors_clicked(QColor color) {
    ui->color->setColor(color);
    emit findFiles(color,ui->deviation->value());
}

void MainWindow::on_color_colorSelected(QColor color) {
    emit findFiles(color,ui->deviation->value());
}

void MainWindow::on_deviation_sliderReleased() {
    emit findFiles(ui->color->color(),ui->deviation->value());
}

void MainWindow::currentImageChanged(QModelIndex current,QModelIndex)
{
    QString path = current.model()->index(current.row(),SearchResultModel::Path).data().toString();
    QList<QColor> colors = SearchResultModel::variantListToColorList(current.model()->index(current.row(),SearchResultModel::Colors).data().toList());

    //qDebug() << colors;

    ui->colors->setColors(colors);

    if (path.isEmpty())
        return;

    setWindowTitle(QString("Picture Search %1 - %2 ").arg(PICTURE_SEARCH_VERSION_STR).arg(QDir::toNativeSeparators(path)));

    //QImage image(path);
    //ui->image->setPixmap(QPixmap::fromImage(image));
    ui->image->setImage(path);
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


void MainWindow::on_analyzeImage_triggered() {
    QStringList formats;
    foreach (QByteArray format, QImageReader::supportedImageFormats())
        formats.append("*." + format);

    QString file = QFileDialog::getOpenFileName(this,QString(),QString(),tr("Image files (%1)").arg(formats.join(" ")));
    if (file.isEmpty())
        return ;

    ImageDialog dialog(file);
    dialog.exec();

#if 0

    QList<QColor> colors;
    for (int i=0;i<10;i++) {
        colors << QColor(rand()%256,rand()%256,rand()%265);
    }
    QAbstractItemModel* model = new ColorsModel(colors,this);

    QTableView* view = new QTableView(this);
    view->setModel(model);
    view->show();
    /*view->horizontalHeader()->setDefaultSectionSize(20);
    view->verticalHeader()->setDefaultSectionSize(20);*/

    view->resizeRowsToContents();
    view->resizeColumnsToContents();
    view->setShowGrid(false);

    QSize s = model->data(model->index(0,0),Qt::SizeHintRole).toSize();
    s.setHeight((s.height()+1)*model->rowCount()+1);
    s.setWidth(s.width()+1);

    qDebug() << view->childrenRect();

    qDebug() << s;

    view->setGeometry(QRect(0,0,30,30));


    view->resizeRowsToContents();
    view->resizeColumnsToContents();
    //QAbstractItemModel* model = view->model();
    QHeaderView* horHeader = view->horizontalHeader();
    QHeaderView* verHeader = view->verticalHeader();

    horHeader->scrollToBottom();
    verHeader->scrollToBottom();

    int rows = model->rowCount();
    int cols = model->columnCount();
    int x = horHeader->sectionViewportPosition(cols-1) + horHeader->offset()
            + horHeader->sectionSize(cols-1) + 1;
    int y = verHeader->sectionViewportPosition(rows-1) + verHeader->offset()
            + verHeader->sectionSize(rows-1) + 1;
    QPoint p = view->viewport()->mapToParent(QPoint(x,y));
    QRect g = view->geometry();
    g.setSize(QSize(p.x(),p.y()));
    view->setGeometry(g);

    qDebug() << p;


    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //view->setFixedSize(s);
#endif

}
