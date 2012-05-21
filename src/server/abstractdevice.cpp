#include "abstractdevice.h"

#include "experiment.h"

AbstractDevice::AbstractDevice(Experiment *exp) :
        QObject(exp)
{
    bus=NoBus;
    numdevices++;
    numdevice=numdevices;
    qDebug()<<"new AbstractDevice created. Device count"<<numdevices;
    factor=0;
    min_value=0;
    max_value=100;
    scale_hint=5;
    unit="UNIT";
    label="LABEL";
    experiment=exp;
}

AbstractDevice::~AbstractDevice() {
    numdevices--;
    qDebug()<<"AbstractDevice"<<numdevice<<"deleted. Device count"<<numdevices;
}


int AbstractDevice::numdevices=0;

int AbstractDevice::deviceNum() const {
    return numdevice;
}

int AbstractDevice::deviceCount() {
    return numdevices;
}

deviceBusType AbstractDevice::busType() const {
    return busType();
}

const QByteArray AbstractDevice::shortName() const {
    return shortname;
}

void AbstractDevice::setShortName(const QByteArray &name) {
    shortname=name;
}

double AbstractDevice::getFactor() const {
    return factor;
}

void AbstractDevice::setFactor(double factor) {
    this->factor=factor;
}

double AbstractDevice::getMinValue() const {
    return min_value;
}

void AbstractDevice::setMinValue(double min_value) {
    this->min_value=min_value;
}

double AbstractDevice::getMaxValue() const {
    return max_value;
}

void AbstractDevice::setMaxValue(double max_value) {
    this->max_value=max_value;
}

double AbstractDevice::getScaleHint() const {
    return scale_hint;
}

void AbstractDevice::setScaleHint(double scale_hint) {
    this->scale_hint=scale_hint;
}

void AbstractDevice::setLabel(QString label) {
    this->label=label;
}

const QString AbstractDevice::getLabel(void) const {
    return label;
}

void AbstractDevice::setUnit(QString unit) {
    this->unit=unit;
}

const QString AbstractDevice::getUnit() const {
    return unit;
}

void AbstractDevice::resetDevice() {
    return;
}

bool AbstractDevice::capable(QString capability) const {
    if (capabilities.contains(capability,Qt::CaseSensitive)) {
        return true;
    } else {
        return false;
    }
}

void AbstractDevice::setCapability(QString capability) {
    if (!capable(capability)) {
        qDebug()<<shortname<<"adding capability"<<capability;
        capabilities.append(capability);
    }
}

QByteArray AbstractDevice::getLastValue() const {
    return lastValue;
}
