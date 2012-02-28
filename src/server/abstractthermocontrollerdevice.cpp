#include "abstractthermocontrollerdevice.h"

AbstractThermocontrollerDevice::AbstractThermocontrollerDevice()

{
    setControlLoopNum(1);

    controlEngaged.fill(false);
    P.fill(0.0);
    I.fill(0.0);
    D.fill(0.0);
    /* Don't need to fill QStrings as they are constructed empty
    controlChannel.fill();
    targetValue.fill();
    */
}


bool AbstractThermocontrollerDevice::isControlEngaged(int controlLoop) const {
    return controlEngaged.at(controlLoop);
}

void AbstractThermocontrollerDevice::engageControl(bool engage, int controlLoop) {
    controlEngaged[controlLoop]=engage;
}

double AbstractThermocontrollerDevice::getP(int controlLoop) const {
    return P.at(controlLoop);
}

double AbstractThermocontrollerDevice::getI(int controlLoop) const {
    return I.at(controlLoop);
}

double AbstractThermocontrollerDevice::getD(int controlLoop) const {
    return D.at(controlLoop);
}

void AbstractThermocontrollerDevice::setP(double p, int controlLoop) {
    if (p>0) {
        P[controlLoop]=p;
    } else {
        qWarning()<<"P should be >0"<<p;
    }
}

void AbstractThermocontrollerDevice::setI(double i, int controlLoop) {
    if (i>0) {
        I[controlLoop]=i;
    } else {
        qWarning()<<"I should be >0"<<i;
    }
}

void AbstractThermocontrollerDevice::setD(double d, int controlLoop) {
    if (d>0) {
        D[controlLoop]=d;
    } else {
        qWarning()<<"D should be >0"<<d;
    }
}

QString AbstractThermocontrollerDevice::getTargetValue(int controlLoop) {
    return targetValue.at(controlLoop);
}

int AbstractThermocontrollerDevice::getControlLoopNum() const {
    return controlLoopNum;
}

QString AbstractThermocontrollerDevice::getLoopName(int num) const {
    return loopName.at(num);
}

int AbstractThermocontrollerDevice::getLoopIndex(QString name) const {
    return loopName.indexOf(name);
}

QStringList AbstractThermocontrollerDevice::getControlTypesList() const {
    return controlTypeList;
}

int AbstractThermocontrollerDevice::getCurrentControlTypeIndex(int loopIndex) const {
    return currentControlType.at(loopIndex);
}

bool AbstractThermocontrollerDevice::setCurrentControlTypeIndex(int typeIndex, int loopIndex) {
    qDebug()<<"Using dummy setCurrentControlTypeIndex implementation of AbstractThermocontrollerDevice";
    currentControlType[loopIndex]=typeIndex;
    return true;
}



void AbstractThermocontrollerDevice::setControlLoopNum(int loops) {
    if (loops<1) return;
    controlLoopNum=loops;
    controlEngaged.resize(loops);
    controlEngaged.fill(false);
    P.resize(loops);
    P.fill(0);
    I.resize(loops);
    I.fill(0);
    D.resize(loops);
    D.fill(0);
    controlChannel.resize(loops);
    targetValue.resize(loops);
    loopName.resize(loops);
    channelName.resize(loops);
    currentControlType.resize(loops);
    currentControlType.fill(0);
}
