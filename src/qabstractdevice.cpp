#include "qabstractdevice.h"


QAbstractDevice::QAbstractDevice(deviceBusType type, QObject *parent) :
        QObject(parent)
{
    qDebug()<<"new QAbstractDevice created with bus type"<<type;
    numdevices++;
    numdevice=numdevices;

    bus=type;

    factor=0;
    min_value=0;
    max_value=100;
    unit="No unit specified";
    label="No label specified";
}

QAbstractDevice::~QAbstractDevice() {
    numdevices--;
    qDebug()<<"QAbstractDevice deleted";
}


int QAbstractDevice::numdevices=0;

int QAbstractDevice::deviceNum() const {
    return numdevice;
}

int QAbstractDevice::deviceCount() {
    return numdevices;
}

deviceBusType QAbstractDevice::busType() const {
    return busType();
}

//const char *QAbstractDevice::shortName() const {
//    return shortname.data();
//}

const QString QAbstractDevice::shortName(void) const {
    return QString(shortname);
}
const double QAbstractDevice::getFactor() const {
    return factor;
}

void QAbstractDevice::setFactor(double factor) {
    this->factor=factor;
}

const double QAbstractDevice::getMinValue() const {
    return min_value;
}

void QAbstractDevice::setMinValue(double min_value) {
    this->min_value=min_value;
}

const double QAbstractDevice::getMaxValue() const {
    return max_value;
}

void QAbstractDevice::setMaxValue(double max_value) {
    this->max_value=max_value;
}

const double QAbstractDevice::getScaleHint() const {
    return scale_hint;
}

void QAbstractDevice::setScaleHint(double scale_hint) {
    this->scale_hint=scale_hint;
}

void QAbstractDevice::setLabel(QString label) {
    this->label=label;
}

const QString QAbstractDevice::getLabel(void) const {
    return label;
}

void QAbstractDevice::setUnit(QString unit) {
    this->unit=unit;
}

const QString QAbstractDevice::getUnit() const {
    return unit;
}
