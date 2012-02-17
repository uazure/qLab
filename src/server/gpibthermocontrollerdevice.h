#ifndef GPIBTHERMOCONTROLLERDEVICE_H
#define GPIBTHERMOCONTROLLERDEVICE_H

#include "abstractthermocontrollerdevice.h"
#include "gpibdevice.h"
#include <QStringList>
#include <QByteArray>

class GpibThermocontrollerDevice : public GpibDevice,
public AbstractThermocontrollerDevice

{
    Q_OBJECT

public:
    GpibThermocontrollerDevice(QByteArray &shortName, QObject *parent=0);
    virtual bool setTargetValue(QString value, int controlLoop=0);
    virtual bool setControlChannel(QString channel, int controlLoop=0);
    virtual QString getControlChannel(int controlLoop=0);

private:
    QString engageControlsCommand,
    disengageControlsCommand,
    statusControlsCommand,
    statusControlsEngagedResponse,
    setControlChannelCommand,
    getControlChannelCommand,
    setTypeControlCommand,
    getTypeControlCommand,
    typesOfControl,
    offControlCommand,
    pidControlCommand,
    getPControlCommand,
    getIControlCommand,
    getDControlCommand,
    setPControlCommand,
    setIControlCommand,
    setDControlCommand,
    getControlTarget,
    setControlTarget;

};

#endif // GPIBTHERMOCONTROLLERDEVICE_H
