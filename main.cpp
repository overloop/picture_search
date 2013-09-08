#include <QApplication>
#include <qglobal.h>

#include "mainwindow.h"
#include "colorextractortest.h"
#include "settingsmodel.h"

/**
 * @todo path case and separator char correctness
 * @todo abort button
 * @todo rewrite about, 'cause it's shit
 * @todo error handling in createTables()
 */

#include "imagestatistics.h"

#include <QtWebKit/QWebView>

int main(int argc, char** argv)
{
    QApplication app(argc,argv);
    qRegisterMetaType<ImageStatisticsList>();

    MainWindow w;
    w.show();

    return app.exec();
}
