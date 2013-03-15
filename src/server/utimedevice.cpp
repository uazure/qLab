#include "utimedevice.h"
#include <QByteArray>

UtimeDevice::UtimeDevice(Experiment *exp) :
        AbstractDevice(exp)
{
    bus=Utime;
    shortname="utime";
    setLabel("Time");
    setUnit("sec");
}

bool UtimeDevice::readValue(QByteArray &returnValue, QStringList) {
    returnValue=QByteArray::number(QDateTime::currentDateTime().toTime_t())+"."+QTime::currentTime().toString("zzz").toLatin1();
    lastValue=returnValue;
    return true;
}

