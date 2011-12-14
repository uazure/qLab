#include "qabstractdevice.h"


QAbstractDevice::QAbstractDevice(deviceBusType type, QObject *parent) :
    QObject(parent)
{
    numdevices++;
    numdevice=numdevices;

    bus=type;

}

QAbstractDevice::~QAbstractDevice() {
    numdevices--;
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

