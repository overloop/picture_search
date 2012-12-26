#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QResizeEvent>
#include "searchresultmodel.h"

#include <QImage>
#include <QPainter>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&indexThread,SIGNAL(progress(int)),ui->progress,SLOT(setValue(int)));
    connect(&searchThread,SIGNAL(found()),this,SLOT(found()));
    connect(&indexThread,SIGNAL(indexStoped()),this,SLOT(indexStoped()));
    //connect(ui->table,SIGNAL(clicked(QModelIndex)),this,SLOT(pictureClicked(QModelIndex)));

    ui->table->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->table->setSelectionMode(QAbstractItemView::SingleSelection);

    //ui->table->setEditTriggers(QAbstractItemView::CurrentChanged);
    //connect(ui->table,SIGNAL(pressed(QModelIndex))

    ui->status->setText("");
    ui->statusbar->addWidget(ui->progress,1);
    ui->statusbar->addWidget(ui->status,1);

    QSizePolicy policy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    policy.setHeightForWidth(true);
    ui->browser->setSizePolicy(policy);

    /*ui->browser->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->browser->setGeometry(QRect(0,0,100,100));*/

    /*QImage image(10,10,QImage::Format_RGB32);
    QPainter p(&image);
    p.fillRect(QRect(0,0,10,10),QColor(Qt::yellow));
    ui->browser->setPixmap(QPixmap::fromImage(image));*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

#include <QScrollBar>

void MainWindow::resizeEvent(QResizeEvent* event)
{
    /*QRect rect = ui->table->geometry();
    rect.setLeft( rect.left() + 150 );
    //rect.setWidth( rect.width()/2 );

    QScrollBar* bar = ui->table->verticalScrollBar();

    if (bar->isVisible())
        rect.setRight( rect.right() - bar->width() );

    //QPoint p = ui->table->mapToParent(QPoint(0,0));
    //qDebug() << p;

    //rect.translate(p);

    ui->browser->setGeometry(rect);*/

    event->accept();
}

void MainWindow::found()
{
    QList<SearchThread::SearchResult> result = searchThread.result();
    SearchResultModel* model = new SearchResultModel(result);
    delete ui->table->model();
    ui->table->setModel(model);
    connect(ui->table->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(currentImageChanged(QModelIndex,QModelIndex)));

    //ui->table->setColumnHidden(1,true);
}

void MainWindow::on_addFolder_triggered()
{
    QString path = QFileDialog::getExistingDirectory(this);
    if (path.isEmpty())
        return;
    if (QMessageBox::information(this, "Subdirectories", "Add subdirectories", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        indexThread.addDir(path,true);
    } else {
        indexThread.addDir(path,false);
    }
    time.start();
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
    ui->statusbar->showMessage(QString("Index created in %1s").arg(time.elapsed() / 1000));
}

/*
void MainWindow::pictureClicked(QModelIndex index)
{
    QString path = index.model()->index(index.row(),1).data().toString();
    if (path.isEmpty())
        return;

    QImage image(path);
    ui->browser->setPixmap(QPixmap::fromImage(image));
}*/

void MainWindow::on_table_activated(QModelIndex index)
{
    qDebug() << index.row() << "activated";
}

void MainWindow::currentImageChanged(QModelIndex current,QModelIndex)
{
    QString path = current.model()->index(current.row(),1).data().toString();
    if (path.isEmpty())
        return;

    qDebug() << current.row() << " is current";

    QImage image(path);
    ui->browser->setPixmap(QPixmap::fromImage(image));
}
