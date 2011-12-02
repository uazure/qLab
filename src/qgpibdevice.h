#ifndef QGPIBDEVICE_H
#define QGPIBDEVICE_H

//#include "gpib-32.h"
#include "qabstractdevice.h"
#include <QByteArray>

class QGpibDevice : public QAbstractDevice
{
    Q_OBJECT
public:
    explicit QGpibDevice(short int gpib_id, QObject *parent = 0);

private:

    short int id;
    int handle;
    bool getIdn(void);

signals:

public slots:

};

#endif // QGPIBDEVICE_H
