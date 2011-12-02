#include "qgpibdevice.h"

QGpibDevice::QGpibDevice(short gpib_id, QObject *parent) :
    QAbstractDevice (Gpib, parent)

{
    id=gpib_id;

}
