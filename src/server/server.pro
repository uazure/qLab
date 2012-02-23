# -------------------------------------------------
# Project created by QtCreator 2011-12-02T14:55:52
# -------------------------------------------------
QT += core \
    gui \
    network
TARGET = server
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    abstractdevice.cpp \
    gpibdevice.cpp \
    devicefarm.cpp \
    experimentconfigurationselectdialog.cpp \
    timedevice.cpp \
    utimedevice.cpp \
    experiment.cpp \
    tcpserver.cpp \
    udpserver.cpp \
    abstractthermocontrollerdevice.cpp \
    gpibthermocontrollerdevice.cpp \
    controllabledevicelist.cpp \
    controlmanagementdialog.cpp \
    clickablelabel.cpp
INCLUDEPATH += ../../lib/
win32:LIBS += 'd:/develop/qLab/lib/gpib-32.obj'
win32:DEFINES += WIN32
HEADERS += mainwindow.h \
    gpib-32.h \
    abstractdevice.h \
    gpibdevice.h \
    devicefarm.h \
    experimentconfigurationselectdialog.h \
    timedevice.h \
    utimedevice.h \
    experiment.h \
    tcpserver.h \
    udpserver.h \
    abstractthermocontrollerdevice.h \
    gpibthermocontrollerdevice.h \
    controllabledevicelist.h \
    controlmanagementdialog.h \
    clickablelabel.h
FORMS += mainwindow.ui \
    experimentconfigurationselectdialog.ui \
    controlmanagementdialog.ui
OTHER_FILES += ../../README.txt \
    ../../lib/gpib-32.dll \
    ../../lib/gpib-32.obj
