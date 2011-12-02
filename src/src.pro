#-------------------------------------------------
#
# Project created by QtCreator 2011-12-02T14:55:52
#
#-------------------------------------------------

QT       += core gui

TARGET = server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qabstractdevice.cpp \
    qgpibdevice.cpp

HEADERS  += mainwindow.h \
    gpib-32.h \
    qabstractdevice.h \
    qgpibdevice.h

INCLUDEPATH += ../lib/ \

win32:LIBS += -L../lib/ -lgpib-32.obj

FORMS    += mainwindow.ui




