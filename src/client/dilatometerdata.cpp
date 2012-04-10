#include "dilatometerdata.h"
#include <QSettings>
#include <QVariant>
#include <QDebug>
#include "thermalexpansionpoint.h"


DilatometerData::DilatometerData(QObject *parent) :
    QObject(parent)
{
    QSettings settings;
    bool ok;
    L0=settings.value("dilatometry/L0",QVariant(1.0)).toDouble(&ok);
    if (!ok) {
        qWarning("Failed read value of L0 from settings. L0=1; Change it manually and save");
        L0=1;
    }
}



bool DilatometerData::setThermalExpansion(double T0, double T1, double dF, double Favg, double tau1, double tau2) {
    try {
        bool onHeat;
        double dL=dF/sensitivity(Favg);
        double dT=T1-T0;
        double Tavg=(T1+T0)/2;
        if (T1>T0) onHeat=true;
        else onHeat=false;
        double alpha=dL/dT*L0;
        ThermalExpansionPoint p(alpha,Tavg,tau1,tau2,onHeat);
        qDebug()<<"Adding new thermal expansion point to set: alpha ="<<alpha<<"T ="<<Tavg<<"tau1 ="<<tau1<<"tau2 ="<<tau2<<"onHeat ="<<onHeat;
        thermalExpansionVector.append(p);
        return true;
    } catch (...) {
        qWarning()<<"Failed to calculate alpha";
        return false;
    }
}

void DilatometerData::setL0(double L0) {
    this->L0=L0;
    qDebug()<<"DilatometerData L0 set to"<<L0;
    QSettings settings;
    settings.setValue("dilatometry/L0",QVariant(L0));
    qDebug()<<"L0 set to"<<L0<<"and saved to file";
}

void DilatometerData::deletePoint(int index) {
    if (index<0 || index>=size()) {
        return;
    }
    thermalExpansionVector.remove(index);
}
