# -------------------------------------------------
# Project created by QtCreator 2012-01-16T13:39:54
# -------------------------------------------------
QT += widgets
QT += core \
    gui \
    network
TARGET = client
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    plot.cpp \
    tcpclient.cpp \
    experimentdata.cpp \
    dataviewwindow.cpp \
    seriesdata.cpp \
    scalemagnifier.cpp \
    scaleplotmagnifier.cpp \
    setupcurvesdialog.cpp \
    ColorBox.cpp \
    scaleplotpanner.cpp \
    tableview.cpp \
    experiment.cpp \
    experimentcontroldialog.cpp \
    ../server/clickablelabel.cpp \
    selectcurvedialog.cpp \
    nonlinearapproximation.cpp \
    approximationdialog.cpp \
    thermalexpansionpoint.cpp \
    dilatometerdata.cpp \
    basicconfigurationdialog.cpp \
    doubleclickeventfilter.cpp \
    aboutdialog.cpp \
    tolerancealarm.cpp \
    stringlistselectdialog.cpp
HEADERS += mainwindow.h \
    plot.h \
    tcpclient.h \
    experimentdata.h \
    dataviewwindow.h \
    seriesdata.h \
    scalemagnifier.h \
    scaleplotmagnifier.h \
    setupcurvesdialog.h \
    ColorBox.h \
    scaleplotpanner.h \
    tableview.h \
    experiment.h \
    experimentcontroldialog.h \
    ../server/clickablelabel.h \
    selectcurvedialog.h \
    nonlinearapproximation.h \
    approximationdialog.h \
    thermalexpansionpoint.h \
    dilatometerdata.h \
    basicconfigurationdialog.h \
    doubleclickeventfilter.h \
    aboutdialog.h \
    tolerancealarm.h \
    stringlistselectdialog.h
FORMS += mainwindow.ui \
    dataviewwindow.ui \
    setupcurvesdialog.ui \
    experimentcontroldialog.ui \
    selectcurvedialog.ui \
    approximationdialog.ui \
    basicconfigurationdialog.ui \
    aboutdialog.ui \
    stringlistselectdialog.ui
INCLUDEPATH += ../qwt6

# INCLUDEPATH += ../../../qwt/src
# qwt 6.0.1 need to be compiled and installed to system path
# LIBS += -L../../../qwt/lib
LIBS += -L../../lib
INCLUDEPATH += "c:/Program Files/GnuWin32/include"
LIBS += -lgsl \
    -lgslcblas

# release mode and unix
LIBS += -lqwt
