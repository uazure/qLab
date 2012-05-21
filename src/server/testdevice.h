#ifndef TESTDEVICE_H
#define TESTDEVICE_H


#include "abstractdevice.h"
#include "experiment.h"

class TestDevice : public AbstractDevice
{
    Q_OBJECT;

public:
    explicit TestDevice(Experiment * exp);
    bool readValue(QByteArray &returnValue, QStringList parameters);
};

#endif // TESTDEVICE_H
