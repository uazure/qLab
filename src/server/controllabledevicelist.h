#ifndef CONTROLLABLEDEVICELIST_H
#define CONTROLLABLEDEVICELIST_H

#include "abstractthermocontrollerdevice.h"

class ControllableDeviceList
{
public:
    ControllableDeviceList();
    void appendControl(AbstractThermocontrollerDevice * device,int channel=0);
    void removeControl(AbstractThermocontrollerDevice * device,int channel=-1);
    void clear();
    AbstractThermocontrollerDevice * getControlDevice(int index);
    int getControlChannel(int index) const;


private:
    QList<AbstractThermocontrollerDevice *> deviceList;
    QMap<int, int> deviceControlMap;

};

#endif // CONTROLLABLEDEVICELIST_H
