#include "utimedevice.h"
#include <QByteArray>

UtimeDevice::UtimeDevice(QObject *parent) :
        AbstractDevice(parent)
{
    bus=Utime;
    shortname="utime";
    setLabel("Time");
    setUnit("sec");
}

bool UtimeDevice::readValue(QByteArray &returnValue, QStringList) {
    returnValue=QByteArray::number(QDateTime::currentDateTime().toTime_t())+"."+QTime::currentTime().toString("zzz").toAscii();
    return true;
}

