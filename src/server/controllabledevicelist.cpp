#include "controllabledevicelist.h"

ControllableDeviceList::ControllableDeviceList()
{
}

void ControllableDeviceList::clear()
{
    deviceList.clear();
    deviceControlMap.clear();
}

void ControllableDeviceList::appendControl(AbstractThermocontrollerDevice *device, int channel)
{
    if (deviceList.contains(device) || channel<0 || device==NULL) {
        return;
    }

    deviceList.append(device);
    deviceControlMap.insert(deviceList.size()-1,channel);
}

// if channel <0 then we should delete all channels of the specified device
void ControllableDeviceList::removeControl(AbstractThermocontrollerDevice *device, int channel)
{
    int index=0;
    if (deviceList.contains(device)) {
        if(channel<0) {

            while (deviceList.contains(device)) {
                index=deviceList.indexOf(device);
                deviceList.removeAt(index);
                deviceControlMap.remove(index);
            }
        } else {
            for (int i=0;i<deviceList.size();++i) {
                if (deviceControlMap.value(i)==channel) {
                    deviceList.removeAt(i);
                    deviceControlMap.remove(i);
                    break;
                }
            }
        }
    }
}


AbstractThermocontrollerDevice * ControllableDeviceList::getControlDevice(int index) {
    if (index<0 || index>=deviceList.size()) {
        qWarning()<<"Failed to getDevice with index"<<index;
        return NULL;
    }
    return deviceList.at(index);
}

int ControllableDeviceList::getControlChannel(int index) const {
    return deviceControlMap.value(index);
}
