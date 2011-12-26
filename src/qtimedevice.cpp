#include "qtimedevice.h"
#include <QByteArray>

QTimeDevice::QTimeDevice(QObject *parent) :
        QAbstractDevice(parent)
{
    bus=Time;
}

bool QTimeDevice::readValue(QByteArray &returnValue, QStringList &parameters) {
    //QTime time=QTime::currentTime();
    returnValue=QTime::currentTime().toString("H:mm:ss").toAscii();
}
