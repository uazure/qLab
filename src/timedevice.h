#ifndef TIMEDEVICE_H
#define TIMEDEVICE_H

#include "abstractdevice.h"
#include <QTime>
#include <QByteArray>

/**  This class provides device that returns current time in format
HH:mm::ss
  */

class TimeDevice : public AbstractDevice
{
Q_OBJECT
public:
    explicit TimeDevice(QObject *parent = 0);
    /// implementation of pure virtual method of QAbstractDevice
    bool readValue(QByteArray &returnValue, QStringList parameters);

signals:

public slots:

};

#endif // TIMEDEVICE_H
