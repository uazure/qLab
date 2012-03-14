#include "timedevice.h"
#include <QByteArray>

TimeDevice::TimeDevice(Experiment *exp) :
        AbstractDevice(exp)
{
    bus=Time;
    shortname="time";
    setLabel("Time");
    setUnit("H:mm:ss");
}

bool TimeDevice::readValue(QByteArray &returnValue, QStringList) {
    returnValue=QTime::currentTime().toString("H:mm:ss").toAscii();
    lastValue=returnValue;
    return true;
}
