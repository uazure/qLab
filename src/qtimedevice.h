#ifndef QTIMEDEVICE_H
#define QTIMEDEVICE_H

#include "qabstractdevice.h"
#include <QTime>
#include <QByteArray>

class QTimeDevice : public QAbstractDevice
{
Q_OBJECT
public:
    explicit QTimeDevice(QObject *parent = 0);
    bool readValue(QByteArray &returnValue, QStringList &parameters);



signals:

public slots:

};

#endif // QTIMEDEVICE_H
