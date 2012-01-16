#include "timedevice.h"
#include <QByteArray>

TimeDevice::TimeDevice(QObject *parent) :
        AbstractDevice(parent)
{
    bus=Time;
    shortname="time";
    setLabel("Time");
    setUnit("H:mm:ss");
}

bool TimeDevice::readValue(QByteArray &returnValue, QStringList) {
    returnValue=QTime::currentTime().toString("H:mm:ss").toAscii();
    return true;
}
