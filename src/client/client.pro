#-------------------------------------------------
#
# Project created by QtCreator 2012-01-16T13:39:54
#
#-------------------------------------------------

QT       += core gui network

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    plot.cpp \
    tcpclient.cpp

HEADERS  += mainwindow.h \
    plot.h \
    tcpclient.h

FORMS    += mainwindow.ui

INCLUDEPATH += ../qwt6/
LIBS += -L ../../lib/ -lqwt
