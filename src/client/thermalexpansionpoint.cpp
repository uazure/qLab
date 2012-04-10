#include "thermalexpansionpoint.h"

ThermalExpansionPoint::ThermalExpansionPoint()
{
    setX(0);
    setY(0);
    tau1=0;
    tau2=0;
    onHeat=true;
    deltaL=0;
}

ThermalExpansionPoint::ThermalExpansionPoint(double T, double alpha, double dL, double tau1, double tau2, bool onHeat) {
    setX(T);
    setY(alpha);
    this->tau1=tau1;
    this->tau2=tau2;
    this->onHeat=onHeat;
    this->deltaL=dL;
}



