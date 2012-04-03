#include "dilatometerdata.h"
#include <QSettings>
#include <QDebug>
#include "thermalexpansionpoint.h"


DilatometerData::DilatometerData(QObject *parent) :
    QObject(parent)
{
    QSettings settings;
    bool ok;
    L0=settings.value("dilatometer/L0",QVariant(1.0)).toDouble(&ok);
    if (!ok) {
        qWarning("Failed read value of L0 from settings. L0=1; Change it manually and save");
        L0=1;
    }


}



bool DilatometerData::setThermalExpansion(double T0, double T1, double dF, double Favg, double tau1, double tau2, bool onHeat) {
    try {
        double dL=dF/sensitivity(Favg);
        double dT=T1-T0;
        double Tavg=(T1+T0)/2;
        if (T1>T0) onHeat=true;
        else onHeat=false;
        double alpha=dL/dT*L0;
        ThermalExpansionPoint p(alpha,Tavg,tau1,tau2,onHeat);
        thermalExpansionVector.append(p);
        return true;
    } catch (...) {
        qWarning()<<"Failed to calculate alpha";
        return false;
    }
}
