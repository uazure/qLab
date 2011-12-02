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
    qabstractdevice.cpp

HEADERS  += mainwindow.h \
    ../lib/gpib-32.h \
    qabstractdevice.h


FORMS    += mainwindow.ui


