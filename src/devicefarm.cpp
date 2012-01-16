#include "devicefarm.h"

DeviceFarm::DeviceFarm()
{

}

 AbstractDevice * DeviceFarm::getDeviceObject(QByteArray &DeviceShortName) {
    QByteArray shortname=DeviceShortName;
    if (shortname=="time") {
        return new TimeDevice();
    }

    if (shortname=="utime") {
        return new UtimeDevice();
    }

    QSettings *settings=new QSettings(QSettings::IniFormat,QSettings::UserScope,QApplication::organizationName(),"devices");
    QString request=shortname;
    QByteArray bus=settings->value(request.append("/bus")).toByteArray();
    delete settings;
    qDebug()<<"Device:"<<shortname<<"bus:"<<bus;

    if (bus=="gpib") {
        GpibDevice *device=new GpibDevice(shortname);
        return device;
    }

    // if no device was created - return null pointer
    qWarning()<<"Failed to create object for device"<<shortname<<"bus"<<bus;
    return NULL;
}


