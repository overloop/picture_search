#include "directoriesdialog.h"
#include "ui_directoriesdialog.h"

#include "directoriesmodel.h"
#include <QFileDialog>
#include <QDebug>

DirectoriesDialog::DirectoriesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DirectoriesDialog)
{
    ui->setupUi(this);

    DirectoriesModel* model = new DirectoriesModel(this);
    ui->table->setModel(model);


    ui->remove->setEnabled(false);

    ui->table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->table->setColumnWidth(0,60);
    ui->table->horizontalHeader()->setStretchLastSection(true);

    connect(ui->table->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(currentChanged(QModelIndex,QModelIndex)));
    connect(ui->table->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(selectionChanged(QItemSelection,QItemSelection)));
}

DirectoriesDialog::~DirectoriesDialog()
{
    delete ui;
}

void DirectoriesDialog::on_add_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this);
    if (path.isEmpty())
        return;

    QAbstractItemModel* model = ui->table->model();

    int row = model->rowCount();

    QDir dir(path);

    model->insertRows(model->rowCount(),1);
    model->setData(model->index(row,1),dir.absolutePath());
    model->setData(model->index(row,0),Qt::Checked,Qt::CheckStateRole);
}

void DirectoriesDialog::on_remove_clicked()
{
    QModelIndexList list = ui->table->selectionModel()->selectedRows();
    if (list.size()<1)
        return;
    ui->table->model()->removeRow(list.at(0).row());
}

void DirectoriesDialog::currentChanged(QModelIndex current,QModelIndex)
{
    //qDebug() << ui->table->selectionModel()->selectedRows().size();
    //qDebug() << ui->table->selectionModel()->selectedRows();
    //qDebug() << ui->table->selectionModel()->selectedIndexes();

    ui->remove->setEnabled(ui->table->selectionModel()->selectedRows().size()>0);
}

void DirectoriesDialog::selectionChanged(QItemSelection current,QItemSelection)
{
    //qDebug() << current;
    //ui->remove->setEnabled(true);

    ui->remove->setEnabled(current.size()>0);

}

QAbstractItemModel* DirectoriesDialog::model() const
{
    return ui->table->model();
}

bool DirectoriesDialog::rescan() const
{
    return ui->rescan->isChecked();
}

void DirectoriesDialog::on_userMode_clicked(bool userMode)
{
    DirectoriesModel* model = static_cast<DirectoriesModel*>(ui->table->model());
    model->setMode(userMode);
}
