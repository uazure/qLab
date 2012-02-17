#ifndef GPIBTHERMOCONTROLLERDEVICE_H
#define GPIBTHERMOCONTROLLERDEVICE_H

#include "abstractthermocontrollerdevice.h"
#include "gpibdevice.h"
#include <QStringList>

class gpibthermocontrollerdevice : public GpibDevice,
public AbstractThermocontrollerDevice

{
    Q_OBJECT

public:
    gpibthermocontrollerdevice(QByteArray shortName, QObject *parent);

private:
    QString engageControlsCommand,
    disengageControlsCommand,
    statusControlsCommand,
    statusControlsEngegedResponse,
    setTypeControlCommand,
    typesOfControl,
    offControlCommand,
    pidControlCommand,
    getPControlCommand,
    getIControlCommand,
    getDControlCommand,
    setPControlCommand,
    setIControlCommand,
    setDControlCommand;

};

#endif // GPIBTHERMOCONTROLLERDEVICE_H
