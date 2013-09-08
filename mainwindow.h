#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QModelIndex>
#include <QAbstractItemModel>

#include "imagestatistics.h"

#include <QThread>

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

protected:
    /*QObject* m_indexWorker;
    QObject* m_databaseWorker;*/

    QTime m_time;
    QAbstractItemModel* m_settingsModel;
    QWidget* m_about;
    QDialog* m_dialog;

    QThread indexThread;
    QThread databaseThread;

signals:
    void scanDirectories(const QStringList& toAdd, const QStringList &toRemove);
    void findFiles(const QColor& c, int deviation);
    void openDatabase(const QStringList& settings);


private slots:
    //void on_selectDirectories_triggered();

    void on_color_colorSelected(QColor color);
    void on_deviation_sliderReleased();
    void on_openDatabase_triggered();
    void on_about_triggered();

    void currentImageChanged(QModelIndex current, QModelIndex);
    void databaseOpened(bool ok,const QString& error);

    void directoriesSelected(const QStringList& dirs);
    void filesFound(const ImageStatisticsList& files);
    void error(const QString& text);
    void status(const QString& text);

};

#endif // MAINWINDOW_H
