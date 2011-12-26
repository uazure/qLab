# -------------------------------------------------
# Project created by QtCreator 2011-12-02T14:55:52
# -------------------------------------------------
QT += core \
    gui
TARGET = server
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    qabstractdevice.cpp \
    qgpibdevice.cpp \
    devicefarm.cpp \
    experimentconfigurationselectdialog.cpp \
    qtimedevice.cpp \
    qutimedevice.cpp \
    qexperiment.cpp
HEADERS += mainwindow.h \
    gpib-32.h \
    qabstractdevice.h \
    qgpibdevice.h \
    devicefarm.h \
    experimentconfigurationselectdialog.h \
    qtimedevice.h \
    qutimedevice.h \
    qexperiment.h
INCLUDEPATH += ../lib/
win32:LIBS += 'd:/develop/qLab/lib/gpib-32.obj'
win32:DEFINES += WIN32
FORMS += mainwindow.ui \
    experimentconfigurationselectdialog.ui
OTHER_FILES += ../README.txt \
    ../lib/gpib-32.dll \
    ../lib/gpib-32.obj
