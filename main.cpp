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

#if 0
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

//openDb(DatabaseSettings("QSQLITE","localhost","picture_search","root",""));

////////////////////////////////////////

QStringList paths = QStringList() << "I:/tmp3/test/77f59507b07295aed7c24cba7718c7b8.jpg"
                                  << "I:/tmp3/test/780aea5e8d4139d158dfc8a70fffb72c.jpg"
                                  << "I:/tmp3/test/789ec3f2e11de8fa3288affa5cb761f4.jpg"
                                  << "I:/tmp3/test/79ac9ffcc1ab3cbb9f1a81d1d984d1d6.jpg"
                                  << "I:/tmp3/test/80b1d0fb017600863ff23e9cef7f63a1.jpg"
                                  << "I:/tmp3/test/86f5811714fedca0527195837abfeacb.jpg"
                                  << "I:/tmp3/test/89c19eac50eb81607396ef89915099e0.png"
                                  << "I:/tmp3/test/926a8de842ca988ee97e17e33b20b78b.jpg"
                                  << "I:/tmp3/test/92fd5fe8a9c21eb3e5c5470ce0000cf5.jpg"
                                  << "I:/tmp3/test/9c63cbc8cccb74bff7ab3b64308c666d.png"
                                  << "I:/tmp3/test/9d0da511c956c77a54a0f964f3505025.jpg"
                                  << "I:/tmp3/test/9da2aa2c91bbef5994d27942fc39c187.jpg"
                                  << "I:/tmp3/test/9db4d116075366165089cb9a3eb0cfb8.jpg"
                                  << "I:/tmp3/test/9fca21e6ffbac13f5ebf6ee3cacec77f.png"
                                  << "I:/tmp3/test/9fd395eb2946ea01c805bd47b2ad8597.jpg";

QStringList paths = QStringList() << "D:/dl/random/random/1344475786233.jpg"
                                  << "D:/dl/random/random/1345906511172.jpg"
                                  << "D:/dl/random/random/1346402081261.jpg"
                                  << "D:/dl/random/random/1346402695927.jpg"
                                  << "D:/dl/random/random/1346406479080.jpg"
                                  << "D:/dl/random/random/1346674995461.jpg"
                                  << "D:/dl/random/random/1346977204149.jpg"
                                  << "D:/dl/random/random/1347152058391.jpg"
                                  << "D:/dl/random/random/1347166483664.jpg";

ColorExtractorTest test(paths);
test.show();


#endif
