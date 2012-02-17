#include "gpibthermocontrollerdevice.h"

gpibthermocontrollerdevice::gpibthermocontrollerdevice(QByteArray shortName, QObject *parent):
        GpibDevice(shortName, parent),
        AbstractThermocontrollerDevice()

{
    //fixme - reference is ambiguous
    experimentSettings->beginGroup(shortname);
    QString channels=experimentSettings->value("channels").toString();
    QString controls=experimentSettings->value("controls").toString();
    QStringList channelList=channels.split(",",QString::SkipEmptyParts);
    QStringList controlList=controls.split(",",QString::SkipEmptyParts);
    if (controlList.size()>0) {
        qDebug()<<"Setting loop num of"<<shortname<<"to"<<controlList.size();
        setControlLoopNum(controlList.size());
    } else {
        qWarning()<<"Control loop num read from config for"<<shortname<<"is"<<controlList.size();

    }

}
