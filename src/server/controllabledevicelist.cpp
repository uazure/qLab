#include "controllabledevicelist.h"

ControllableDeviceList::ControllableDeviceList()
{
}

void ControllableDeviceList::clear()
{
    deviceList.clear();
    deviceLoopIndexMap.clear();
}

void ControllableDeviceList::appendControl(AbstractThermocontrollerDevice *device, int loopIndex)
{
    if (loopIndex<0 || device==NULL) {
        return;
    }
    deviceList.append(device);
    deviceLoopIndexMap.insert(deviceList.size()-1,loopIndex);
    AbstractDevice *adev=dynamic_cast<AbstractDevice*> (device);
    deviceNameList.append(adev->shortName());
}

// if channel <0 then we should delete all channels of the specified device
void ControllableDeviceList::removeControl(AbstractThermocontrollerDevice *device, int loopIndex)
{
    int index=0;
    if (deviceList.contains(device)) {
        if(loopIndex<0) {

            while (deviceList.contains(device)) {
                index=deviceList.indexOf(device);
                deviceList.removeAt(index);
                deviceLoopIndexMap.remove(index);
                deviceNameList.removeAt(index);
            }
        } else {
            for (int i=0;i<deviceList.size();++i) {
                if (deviceLoopIndexMap.value(i)==loopIndex) {
                    deviceList.removeAt(i);
                    deviceNameList.removeAt(i);
                    deviceLoopIndexMap.remove(i);
                    break;
                }
            }
        }
    }
}


AbstractThermocontrollerDevice * ControllableDeviceList::getControlDevice(int controlIndex) {
    if (controlIndex<0 || controlIndex>=deviceList.size()) {
        qWarning()<<"Failed to getDevice with index"<<controlIndex;
        return NULL;
    }
    return deviceList.at(controlIndex);
}

int ControllableDeviceList::getControlLoopIndex(int controlIndex) const{
    return deviceLoopIndexMap.value(controlIndex);
}

int ControllableDeviceList::size() const {
    return deviceList.size();
}

QStringList ControllableDeviceList::getControlList() {
    QStringList reply;
    for (int i=0;i<deviceList.size();i++) {
        AbstractThermocontrollerDevice *tcdevice=deviceList.at(i);
        AbstractDevice *device=dynamic_cast<AbstractDevice *> (tcdevice);

        if (device==NULL) {
            qWarning()<<"Failed to convert to AbstractDevice control"<<i;
            continue;
        }
        reply.append(QString(device->shortName()));
    }
    return reply;
}

QString ControllableDeviceList::getControlName(int index) const {
    return deviceNameList.at(index);
}
