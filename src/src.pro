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
    devicefarm.cpp
HEADERS += mainwindow.h \
    gpib-32.h \
    qabstractdevice.h \
    qgpibdevice.h \
    devicefarm.h
INCLUDEPATH += ../lib/
win32:LIBS += -L../lib/
win32:LIBS += 'd:/develop/qLab/lib/gpib-32.obj'
FORMS += mainwindow.ui
OTHER_FILES += ../README.txt \
    ../lib/gpib-32.dll \
    ../lib/gpib-32.obj
