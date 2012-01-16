#ifndef UTIMEDEVICE_H
#define UTIMEDEVICE_H

#include "abstractdevice.h"
#include <QTime>
#include <QDateTime>
#include <QByteArray>

/**  This class provides device that returns current time as
unix timestamp with milliseconds resolution
  */
class UtimeDevice : public AbstractDevice
{
Q_OBJECT
public:
    explicit UtimeDevice(QObject *parent = 0);
    /// Implementation of pure virtual method of QAbstractDevice
    bool readValue(QByteArray &returnValue, QStringList parameters=QStringList());

signals:

public slots:

};

#endif // UTIMEDEVICE_H
