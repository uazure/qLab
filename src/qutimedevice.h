#ifndef QUTIMEDEVICE_H
#define QUTIMEDEVICE_H

#include "qabstractdevice.h"
#include <QTime>
#include <QDateTime>
#include <QByteArray>

/**  This class provides device that returns current time as
unix timestamp with milliseconds resolution
  */
class QUtimeDevice : public QAbstractDevice
{
Q_OBJECT
public:
    explicit QUtimeDevice(QObject *parent = 0);
    /// Implementation of pure virtual method of QAbstractDevice
    bool readValue(QByteArray &returnValue, QStringList parameters=QStringList());

signals:

public slots:

};

#endif // QUTIMEDEVICE_H
