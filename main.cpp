#include <QApplication>
#include <qglobal.h>

#include "mainwindow.h"
#include "colorextractortest.h"
#include "settingsmodel.h"

/**
 * @todo path to lowercase
 * @todo cancel button
 * @todo taskbar progress
 * @todo about
 * @todo error handling in createTables()
 */

int main(int argc, char** argv)
{
    QApplication app(argc,argv);

    MainWindow w;
    w.show();

    return app.exec();
}
