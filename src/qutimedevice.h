#ifndef QUTIMEDEVICE_H
#define QUTIMEDEVICE_H

#include "qabstractdevice.h"
#include <QTime>
#include <QDateTime>
#include <QByteArray>

class QUtimeDevice : public QAbstractDevice
{
Q_OBJECT
public:
    explicit QUtimeDevice(QObject *parent = 0);
    bool readValue(QByteArray &returnValue, QStringList parameters);

signals:

public slots:

};

#endif // QUTIMEDEVICE_H
