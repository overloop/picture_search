#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QModelIndex>
#include <QAbstractItemModel>

#include "indexthread.h"
#include "searchthread.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    IndexThread m_indexThread;
    SearchThread m_searchThread;
    QTime m_time;
    QAbstractItemModel* m_settingsModel;
    QWidget* m_about;
    QDialog* m_dialog;

private slots:
    void on_selectDirectories_triggered();
    void on_color_colorSelected(QColor color);
    void on_deviation_valueChanged(int value);
    void on_openDatabase_triggered();
    void on_about_triggered();

    void found();
    void indexStoped();
    void currentImageChanged(QModelIndex current, QModelIndex);
    void databaseOpened(QString error);

};

#endif // MAINWINDOW_H
