#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include "searchresultmodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&indexThread,SIGNAL(progress(int)),ui->progress,SLOT(setValue(int)));
    connect(&searchThread,SIGNAL(found()),this,SLOT(found()));
    ui->status->setText("");
    ui->statusbar->addWidget(ui->progress,1);
    ui->statusbar->addWidget(ui->status,1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::found()
{
    QList<SearchThread::SearchResult> result = searchThread.result();
    SearchResultModel* model = new SearchResultModel(result);
    delete ui->table->model();
    ui->table->setModel(model);
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
}

void MainWindow::on_color_colorSelected(QColor color)
{
    searchThread.search(color,ui->deviation->value());
}

void MainWindow::on_deviation_valueChanged(int value)
{
    searchThread.search(ui->color->color(),value);
}
