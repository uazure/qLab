#ifndef DEVICEFARM_H
#define DEVICEFARM_H
#include "qabstractdevice.h"
#include "qgpibdevice.h"
#include "qtimedevice.h"
#include <QByteArray>
#include <QSettings>
#include <QApplication>

/** The only purpose of this class is to generate objects of specific
classes that are children of QAbstractDevice and are providing
functionaltity required for performing measurements
  */

class DeviceFarm
{
public:
    DeviceFarm();
    /** Main function. Returns pointer to object of QAbstractDevice type
      based on the bus type of the device.

      For example it creates QGpibDevice object for gpib bus type.
      QGpibDevice then takes care about low-level operations and
      provides unified interface for measured data. (i.e. implements
      readValue() method of QAbstractDevice class.

      This method returns pointer to object on success or null on fail.
      */

    static QAbstractDevice * getDeviceObject(QByteArray &DeviceShortName);
};

#endif // DEVICEFARM_H
