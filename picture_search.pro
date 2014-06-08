QT+= \
    sql

include(taskbarprogress/taskbarprogress.pri)

INCLUDEPATH += forms

SOURCES += \
    main.cpp \
    colorpushbutton.cpp \
    mainwindow.cpp \
    colorselectpushbutton.cpp \
    searchresultmodel.cpp \
    colorextractortest.cpp \
    imageview.cpp \
    directoriesdialog.cpp \
    opendatabasedialog.cpp \
    databasesettings.cpp \
    NEUQUANT.C \
    colorextractorsimple.cpp \
    colorextractor.cpp \
    colorextractorneuquant.cpp \
    settingsmodel.cpp \
    about.cpp \
    indexworker.cpp \
    databaseworker.cpp \
    imagestatistics.cpp \
    imagelabel.cpp \
    colorsmodel.cpp \
    forms/imagedialog.cpp \
    forms/colorsview.cpp \
    colorswidget.cpp

HEADERS += \
    colorpushbutton.h \
    mainwindow.h \
    colorselectpushbutton.h \
    searchresultmodel.h \
    colorextractortest.h \
    imageview.h \
    directoriesdialog.h \
    opendatabasedialog.h \
    databasesettings.h \
    NEUQUANT.H \
    colorextractorsimple.h \
    colorextractor.h \
    colorextractorneuquant.h \
    settingsmodel.h \
    about.h \
    indexworker.h \
    databaseworker.h \
    imagestatistics.h \
    picture_search.h \
    imagelabel.h \
    colorsmodel.h \
    forms/imagedialog.h \
    forms/colorsview.h \
    colorswidget.h

FORMS += \
    mainwindow.ui \
    colorextractortest.ui \
    directoriesdialog.ui \
    opendatabasedialog.ui \
    about.ui \
    forms/imagedialog.ui

RC_FILE = \
	picture_search.rc

OTHER_FILES += \
    picture_search.rc











