#ifndef QGPIBDEVICE_H
#define QGPIBDEVICE_H

#include "gpib-32.h"
#include "qabstractdevice.h"
#include <QDebug>
#include <QByteArray>
#include <QSettings>

class QGpibDevice : public QAbstractDevice
{
    Q_OBJECT
public:
    explicit QGpibDevice(QByteArray &DeviceShortName, QObject *parent = 0);
    int Id(void) const;
    int Handle(void) const;
    bool readValue(int channel, QByteArray &returnValue);

    bool isOnline(void);
    void resetDevice();

private:

    /// gpib external variables allocation
    int ibsta,iberr,ibcnt;
    long int ibcntl;

    /// gpib id
    short int id;

    /// current gpib channel
    int channel;

    /// handle of the device
    int handle;

    bool set (QByteArray command);
    bool get (QByteArray *reply);
    bool ask (QByteArray command, QByteArray *reply);
    bool getIdn(void);


signals:

public slots:

};

#endif // QGPIBDEVICE_H
