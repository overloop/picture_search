#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QModelIndex>

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
    IndexThread indexThread;
    SearchThread searchThread;
    QTime time;

private slots:
    void on_addFolder_triggered();
    void on_color_colorSelected(QColor color);
    void on_deviation_valueChanged(int value);
    void on_table_activated(QModelIndex);

    void found();
    void indexStoped();
    void currentImageChanged(QModelIndex current, QModelIndex);

};

#endif // MAINWINDOW_H
