#ifndef GPIBTHERMOCONTROLLERDEVICE_H
#define GPIBTHERMOCONTROLLERDEVICE_H

#include "abstractthermocontrollerdevice.h"
#include "gpibdevice.h"

class gpibthermocontrollerdevice : public AbstractThermocontrollerDevice,
virtual public GpibDevice

{
    Q_OBJECT

public:
    gpibthermocontrollerdevice(QByteArray shortName, QObject *parent);

};

#endif // GPIBTHERMOCONTROLLERDEVICE_H
