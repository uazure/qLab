#include "thermalexpansionpoint.h"

ThermalExpansionPoint::ThermalExpansionPoint()
{
    setX(0);
    setY(0);
    tau1=0;
    tau2=0;
    onHeat=true;
}

ThermalExpansionPoint::ThermalExpansionPoint(double alpha, double T, double tau1, double tau2, bool onHeat) {
    setX(T);
    setY(alpha);
    this->tau1=tau1;
    this->tau2=tau2;
    this->onHeat=onHeat;
}



