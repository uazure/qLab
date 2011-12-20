#include "devicefarm.h"

DeviceFarm::DeviceFarm()
{

}

static QAbstractDevice * DeviceFarm::getDeviceObject(char *DeviceShortName) {
    QByteArray shortname=DeviceShortName;
    QSettings settings;
    QString request=shortname;
    QByteArray bus=settings.value(request.append("/bus"));
    qDebug()<<"Device:"<<shortname<<"bus:"<<bus;

    if (bus=="gpib") {
        QGpibDevice *device=new QGpibDevice(shortname);
        return device;
    }

    // if no device was created - return null pointer
    return null;
}


