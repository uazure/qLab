#ifndef CONTROLLABLEDEVICELIST_H
#define CONTROLLABLEDEVICELIST_H

#include "abstractthermocontrollerdevice.h"

class ControllableDeviceList
{
public:
    ControllableDeviceList();
    void appendControl(AbstractThermocontrollerDevice * device,int loopIndex=0);
    void removeControl(AbstractThermocontrollerDevice * device,int loopIndex=-1);
    void clear();
    AbstractThermocontrollerDevice * getControlDevice(int controlIndex);
    int getControlLoopIndex(int controlIndex) const;


private:
    QList<AbstractThermocontrollerDevice *> deviceList;
    QMap<int, int> deviceLoopIndexMap;

};

#endif // CONTROLLABLEDEVICELIST_H
