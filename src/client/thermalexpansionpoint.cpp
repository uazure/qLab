#include "thermalexpansionpoint.h"

ThermalExpansionPoint::ThermalExpansionPoint()
{
    T=0;
    alpha=0;
    tau1=0;
    tau2=0;
}

ThermalExpansionPoint::ThermalExpansionPoint(double alpha, double T, double tau1, double tau2) {
    this->T=T;
    this->alpha=alpha;
    this->tau1=tau1;
    this->tau2=tau2;
}



