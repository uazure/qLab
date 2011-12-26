#include "qutimedevice.h"
#include <QByteArray>

QUtimeDevice::QUtimeDevice(QObject *parent) :
        QAbstractDevice(parent)
{
    bus=Utime;
}

bool QUtimeDevice::readValue(QByteArray &returnValue, QStringList &parameters) {
    returnValue=QByteArray::number(QDateTime().toTime_t());
}
