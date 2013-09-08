#include "directoriesdialog.h"
#include "ui_directoriesdialog.h"

#include "directoriesmodel.h"
#include <QFileDialog>
#include <QDebug>

#include <QStandardItemModel>

DirectoriesDialog::DirectoriesDialog(const QStringList &dirs, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DirectoriesDialog)
{
    ui->setupUi(this);

    /*DirectoriesModel* model = new DirectoriesModel(this);*/


    int n = dirs.size();
    QStandardItemModel* model = new QStandardItemModel(n,1,this);
    for (int i=0;i<n;i++) {
        model->setData(model->index(i,0),dirs.at(i));
        m_dirsBefore.insert(dirs.at(i));
    }
    model->setHeaderData(0,Qt::Horizontal,QString("path"));

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
    model->setData(model->index(row,0),dir.absolutePath());
}

void DirectoriesDialog::on_remove_clicked()
{
    QModelIndexList list = ui->table->selectionModel()->selectedRows();
    if (list.size()<1)
        return;
    ui->table->model()->removeRow(list.at(0).row());
}

void DirectoriesDialog::currentChanged(QModelIndex,QModelIndex)
{
    ui->remove->setEnabled(ui->table->selectionModel()->selectedRows().size()>0);
}

void DirectoriesDialog::selectionChanged(QItemSelection current,QItemSelection)
{
    ui->remove->setEnabled(current.size()>0);
}

QStringList DirectoriesDialog::directoriesToAdd() const {

    QStringList dirs;
    QString dir;
    foreach(dir,m_dirsAfter) {
        if (m_dirsBefore.contains(dir)) {
            if (ui->rescan->isChecked())
                dirs << dir;
        } else {
            dirs << dir;
        }
    }
    return dirs;
}

QStringList DirectoriesDialog::directoriesToRemove() const {
    QStringList dirs;
    QString dir;
    foreach(dir,m_dirsBefore) {
        if (!m_dirsAfter.contains(dir))
            dirs << dir;
    }
    return dirs;
}


void DirectoriesDialog::accept() {

    QAbstractItemModel* model = ui->table->model();
    int n = model->rowCount();
    for (int i=0;i<n;i++)
        m_dirsAfter.insert(model->data(model->index(i,0)).toString());

    QDialog::accept();
}

/*
void DirectoriesDialog::on_userMode_clicked(bool userMode)
{
    DirectoriesModel* model = static_cast<DirectoriesModel*>(ui->table->model());
    model->setMode(userMode);
}*/
