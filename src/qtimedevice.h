#ifndef QTIMEDEVICE_H
#define QTIMEDEVICE_H

#include "qabstractdevice.h"
#include <QTime>
#include <QByteArray>

/**  This class provides device that returns current time in format
HH:mm::ss
  */

class QTimeDevice : public QAbstractDevice
{
Q_OBJECT
public:
    explicit QTimeDevice(QObject *parent = 0);
    /// implementation of pure virtual method of QAbstractDevice
    bool readValue(QByteArray &returnValue, QStringList parameters);

signals:

public slots:

};

#endif // QTIMEDEVICE_H
