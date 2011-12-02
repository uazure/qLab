#include "qgpibdevice.h"

QGpibDevice::QGpibDevice(short gpib_id, QObject *parent) :
    QAbstractDevice (Gpib, parent)

{
    id=gpib_id;


}

bool QGpibDevice::getIdn() {
    QByteArray query("IDN?");
    //FIXME: here we need to call low-level gpib commads to get IDN string from the device
    return true;
}
