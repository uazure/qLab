#include "gpibthermocontrollerdevice.h"

GpibThermocontrollerDevice::GpibThermocontrollerDevice(QByteArray &shortName, QObject *parent):
        GpibDevice(shortName, parent),
        AbstractThermocontrollerDevice()

{
    setCapability("thermocontroller");
    //entering device options group to read values
    deviceSettings->beginGroup(shortname);
    //reading channels string
    QString channels=deviceSettings->value("channels").toString();
    //reading controls string
    QString controls=deviceSettings->value("controls").toString();
    //split comma-separated values in string to stringlist
    QStringList channelList=channels.split(",",QString::SkipEmptyParts);
    QStringList controlList=controls.split(",",QString::SkipEmptyParts);

    //set controlloopnum to the controlList.size()
    if (controlList.size()>0) {
        qDebug()<<"Setting loop num of"<<shortname<<"to"<<controlList.size();
        setControlLoopNum(controlList.size());
    } else {
        qWarning()<<"Control loop num read from config for"<<shortname<<"is"<<controlList.size();
    }

    //setting loop names as they should appear in gpib commands
    for (int i=0;i<controlList.size();++i) {
        loopName[i]=controlList.at(i);
    }

    //initializing gpib commands from config
    engageControlsCommand=deviceSettings->value("control/engage").toString();
    disengageControlsCommand=deviceSettings->value("control/disengage").toString();
    statusControlsCommand=deviceSettings->value("control/status").toString();
    statusControlsEngagedResponse=deviceSettings->value("control/status-on").toString();
    setTypeControlCommand=deviceSettings->value("control-set/type").toString();
    getTypeControlCommand=deviceSettings->value("control/type").toString();
    offControlCommand=deviceSettings->value("control/type-off").toString();
    pidControlCommand=deviceSettings->value("control/type-pid").toString();
    getControlTarget=deviceSettings->value("control/type").toString();
    setControlTarget=deviceSettings->value("control-set/type").toString();
    getPControlCommand=deviceSettings->value("control/P").toString();
    getIControlCommand=deviceSettings->value("control/I").toString();
    getDControlCommand=deviceSettings->value("control/D").toString();
    setPControlCommand=deviceSettings->value("control-set/P").toString();
    setIControlCommand=deviceSettings->value("control-set/I").toString();
    setDControlCommand=deviceSettings->value("control-set/D").toString();
    getControlChannelCommand=deviceSettings->value("control/channel").toString();
    setControlChannelCommand=deviceSettings->value("control-set/channel").toString();
}

bool GpibThermocontrollerDevice::setTargetValue(QString value, int controlLoop) {
    QString command=setControlTarget.arg(loopName.at(controlLoop),value).toAscii();
    qDebug()<<shortname<<"issue command"<<command;
    if (set(command.toAscii())) {
        qDebug()<<shortname<<"target for loop"<<loopName.at(controlLoop)<<"set"<<value;
        targetValue[controlLoop]=value;
        return true;
    } else {
        qWarning()<<shortname<<"Failed to set target value"<<value;
        return false;
    }
}

bool GpibThermocontrollerDevice::setControlChannel(QString channel, int controlLoop) {
    QString command=setControlChannelCommand.arg(loopName.at(controlLoop),channel);
    qDebug()<<shortname<<"issue command"<<command;
    if (set(command.toAscii())) {
        qDebug()<<shortname<<"channel for loop"<<loopName.at(controlLoop)<<"set"<<channel;
        controlChannel[controlLoop]=channel;
        return true;
    } else {
        qWarning()<<shortname<<"Failed to set channel"<<channel;
        return false;
    }
}

QString GpibThermocontrollerDevice::getControlChannel(int controlLoop) {
    if (controlLoop<0) {
        qWarning()<<"Was trying to get control channel of loop"<<controlLoop;
        return "";
    }
    if (controlLoop>=controlChannel.size()) {
        qWarning()<<"Was trying to get control channel of loop"<<controlLoop;
        return "";
    }

    return controlChannel.at(controlLoop);
}
