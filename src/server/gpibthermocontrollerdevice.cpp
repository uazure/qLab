#include "gpibthermocontrollerdevice.h"

gpibthermocontrollerdevice::gpibthermocontrollerdevice(QByteArray shortName, QObject *parent):
        AbstractThermocontrollerDevice(parent),
        GpibDevice(shortName, parent)
{
    //fixme - reference is ambiguous
//    QString channels=experimentSettings->value(shortname+"/channels").toString();
//    QString controls=experimentSettings->value(shortname+"/controls").toString();


}
