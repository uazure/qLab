#ifndef QABSTRACTDEVICE_H
#define QABSTRACTDEVICE_H

#include <QObject>

class QAbstractDevice : public QObject
{
    Q_OBJECT
public:
    explicit QAbstractDevice(QObject *parent = 0);

signals:

public slots:

};

#endif // QABSTRACTDEVICE_H
