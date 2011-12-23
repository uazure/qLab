#include "qtimedevice.h"
#include <QByteArray>

QTimeDevice::QTimeDevice(QObject *parent) :
        QAbstractDevice(Time, parent)
{
}

bool QTimeDevice::readValue(QByteArray &returnValue, QStringList &parameters) {
    QTime time=QTime::currentTime();
    returnValue=time.toString("H:mm:ss").toAscii();
}
