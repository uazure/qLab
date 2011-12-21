#include "qabstractdevice.h"


QAbstractDevice::QAbstractDevice(deviceBusType type, QObject *parent) :
        QObject(parent)
{
    qDebug()<<"new QAbstractDevice created with bus type"<<type;
    numdevices++;
    numdevice=numdevices;

    bus=type;

    factor=0;
    min_value=0;
    max_value=100;
    unit="No unit specified";
    label="No label specified";
}

QAbstractDevice::~QAbstractDevice() {
    numdevices--;
    qDebug()<<"QAbstractDevice deleted";
}


int QAbstractDevice::numdevices=0;

int QAbstractDevice::deviceNum() const {
    return numdevice;
}

int QAbstractDevice::deviceCount() {
    return numdevices;
}

deviceBusType QAbstractDevice::busType() const {
    return busType();
}

const char *QAbstractDevice::shortName() const {
    return shortname.data();
}
