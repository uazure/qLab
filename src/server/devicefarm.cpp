#include "devicefarm.h"

DeviceFarm::DeviceFarm()
{

}

 AbstractDevice * DeviceFarm::getDeviceObject(QByteArray &DeviceShortName,Experiment *exp) {
    QByteArray shortname=DeviceShortName;
    if (shortname=="time") {
        return new TimeDevice(exp);
    }

    if (shortname=="utime") {
        return new UtimeDevice(exp);
    }

    if (shortname.startsWith("cross")) {
        return new CrossDevice(exp);
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
            device=new GpibThermocontrollerDevice(shortname,exp);
        } else {
            device=new GpibDevice(shortname,exp);
        }
    return device;

    }

    // if no device was created - return null pointer
    qWarning()<<"Failed to create object for device"<<shortname<<"bus"<<bus;
    return NULL;
}


