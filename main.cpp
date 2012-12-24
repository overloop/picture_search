#include <QApplication>

#include "indexthread.h"

#include <QProgressBar>
#include <QSqlDatabase>

//#include "colorpushbutton.h"
#include "mainwindow.h"


void openDb();

int main(int argc, char** argv)
{
    QApplication app(argc,argv);

    /*QProgressBar progress;
    progress.setMinimum(0);
    progress.setMaximum(1000);

    IndexThread thread;
    QObject::connect(&thread,SIGNAL(progress(int)),&progress,SLOT(setValue(int)));
    //QObject::connect(&thread,SIGNAL(indexationFinished()),&progress,SLOT(hide()));
    progress.show();
    thread.addDir("I:/tmp3/test", true);*/

    /*ColorPushButton button;
    button.show();*/

    openDb();
    MainWindow w;
    w.show();

    return app.exec();
}

void openDb() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setDatabaseName("picture_search");
    db.setHostName("localhost");
    db.setUserName("root");
    Q_ASSERT(db.open());
}
