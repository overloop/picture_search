QT+= \
    sql

include(taskbarprogress/taskbarprogress.pri)

SOURCES += \
    main.cpp \
    indexthread.cpp \
    indexjob.cpp \
    indexjobs.cpp \
    searchthread.cpp \
    sqlquery.cpp \
    colorpushbutton.cpp \
    mainwindow.cpp \
    colorselectpushbutton.cpp \
    searchresultmodel.cpp \
    colorextractortest.cpp \
    pixmaplabel.cpp \
    imageview.cpp \
    directoriesmodel.cpp \
    directoriesdialog.cpp \
    opendatabasedialog.cpp \
    databasesettings.cpp \
    NEUQUANT.C \
    colorextractorsimple.cpp \
    colorextractor.cpp \
    colorextractorneuquant.cpp \
    settingsmodel.cpp \
    database.cpp \
    about.cpp \
    indexworker.cpp \
    databaseworker.cpp \
    imagestatistics.cpp

HEADERS += \
    indexthread.h \
    indexjob.h \
    indexjobs.h \
    searchthread.h \
    sqlquery.h \
    colorpushbutton.h \
    mainwindow.h \
    colorselectpushbutton.h \
    searchresultmodel.h \
    colorextractortest.h \
    pixmaplabel.h \
    imageview.h \
    directoriesmodel.h \
    directoriesdialog.h \
    opendatabasedialog.h \
    databasesettings.h \
    NEUQUANT.H \
    colorextractorsimple.h \
    colorextractor.h \
    colorextractorneuquant.h \
    settingsmodel.h \
    database.h \
    about.h \
    indexworker.h \
    databaseworker.h \
    imagestatistics.h \
    picture_search.h

FORMS += \
    mainwindow.ui \
    colorextractortest.ui \
    directoriesdialog.ui \
    opendatabasedialog.ui \
    about.ui

RC_FILE = \
	picture_search.rc

OTHER_FILES += \
    picture_search.rc











