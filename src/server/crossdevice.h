#ifndef CROSSDEVICE_H
#define CROSSDEVICE_H

#include "abstractdevice.h"
#include "experiment.h"

class CrossDevice : public AbstractDevice
{
    Q_OBJECT;

public:
    explicit CrossDevice(Experiment * exp);
    bool readValue(QByteArray &returnValue, QStringList parameters);
private:
    AbstractDevice *dev1, *dev2;


};

#endif // CROSSDEVICE_H
