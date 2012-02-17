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
    settings->beginGroup(QString(shortname));
    QByteArray bus=settings->value("bus").toByteArray();
    bool isThermocontroller=settings->value("thermocontroller").toBool();

    delete settings;
    qDebug()<<"Device:"<<shortname<<"bus:"<<bus;

    GpibDevice *device;

    if (bus=="gpib") {
        if (isThermocontroller) {
            device=new GpibThermocontrollerDevice(shortname);
        } else {
            device=new GpibDevice(shortname);
        }
    return device;

    }

    // if no device was created - return null pointer
    qWarning()<<"Failed to create object for device"<<shortname<<"bus"<<bus;
    return NULL;
}


