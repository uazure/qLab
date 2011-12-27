#include "qutimedevice.h"
#include <QByteArray>

QUtimeDevice::QUtimeDevice(QObject *parent) :
        QAbstractDevice(parent)
{
    bus=Utime;
}

bool QUtimeDevice::readValue(QByteArray &returnValue, QStringList) {
    returnValue=QByteArray::number(QDateTime::currentDateTime().toTime_t())+"."+QTime::currentTime().toString("zzz").toAscii();
    return true;
}
