#ifndef CONTROLLABLEDEVICELIST_H
#define CONTROLLABLEDEVICELIST_H

#include <QStringList>
#include "abstractthermocontrollerdevice.h"
#include "abstractdevice.h"

/** Terminology:
    CONTROL - part of experiment. Experiment can have 0 or more controls.
    Control is defined by AbstractThermocontroller pointer and it's LOOP
    index

    LOOP - part of thermocontroller device. Loop has name and index.

    LOOP NAME - it's used when issueing commands to the thermocontroller
    device

    LOOP INDEX - it's used to reference LOOP.
  */

class ControllableDeviceList
{
public:
    ControllableDeviceList();
    void appendControl(QString &name,AbstractThermocontrollerDevice * device,int loopIndex=0);
    void removeControl(AbstractThermocontrollerDevice * device,int loopIndex=-1);
    void clear();
    AbstractThermocontrollerDevice * getControlDevice(int controlIndex);
    int getControlLoopIndex(int controlIndex) const;
    QString getControlName(int index) const;
    int size() const;
    QStringList getControlList();


private:
    QList<AbstractThermocontrollerDevice *> deviceList;
    QStringList deviceNameList;
    QMap<int, int> deviceLoopIndexMap;

};

#endif // CONTROLLABLEDEVICELIST_H
