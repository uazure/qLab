#ifndef QGPIBDEVICE_H
#define QGPIBDEVICE_H

#include "gpib-32.h"
#include "qabstractdevice.h"
#include <QDebug>
#include <QByteArray>

class QGpibDevice : public QAbstractDevice
{
    Q_OBJECT
public:
    explicit QGpibDevice(short int gpib_id, QObject *parent = 0);
    int Id(void) const;
    int Handle(void) const;

private:

    short int id;
    int channel;
    int handle;
    bool getIdn(void);
    bool set (QByteArray command);
    bool get (QByteArray *reply);
    bool ask (QByteArray command, QByteArray *reply);


signals:

public slots:

};

#endif // QGPIBDEVICE_H
