#ifndef DIRECTORIESDIALOG_H
#define DIRECTORIESDIALOG_H

#include <QDialog>

#include <QModelIndex>
#include <QItemSelection>

namespace Ui {
class DirectoriesDialog;
}

class DirectoriesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DirectoriesDialog(const QStringList& dirs, QWidget *parent = 0);
    ~DirectoriesDialog();
    
    QStringList directoriesToAdd() const;
    QStringList directoriesToRemove() const;

protected:
    QSet<QString> m_dirsBefore;
    QSet<QString> m_dirsAfter;

private:
    Ui::DirectoriesDialog *ui;

private slots:

    void on_add_clicked();
    void on_remove_clicked();
    /*void on_userMode_clicked(bool userMode);*/

    void currentChanged(QModelIndex,QModelIndex);
    void selectionChanged(QItemSelection,QItemSelection);

    void accept();

};

#endif // DIRECTORIESDIALOG_H
