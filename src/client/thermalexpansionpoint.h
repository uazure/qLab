#ifndef THERMALEXPANSIONPOINT_H
#define THERMALEXPANSIONPOINT_H

/** Class for storing data for single thermal expansion point
  Array of such points can be stored to allow direct access of QwtPlotCurve
  to double* data
  TODO: implement operator < and == to make it possible to sort array of
      ThermalExpansionPoint for adequate
  */
class ThermalExpansionPoint
{
public:
    ThermalExpansionPoint();
    ThermalExpansionPoint(double alpha, double T, double tau1=0, double tau2=0);

    inline double getT() const {return T;}
    inline double getAlpha() const {return alpha;}
    inline double getTau1() const {return tau1;}
    inline double getTau2() const {return tau2;}
    inline bool isOnHeat() const {return onHeat;}
    inline void setT(double temp) {T=temp;}
    inline void setAlpha(double a) {alpha=a;}
    inline void setTau1(double t1) {tau1=t1;}
    inline void setTau2(double t2) {tau2=t2;}
    inline void setOnHeat (bool heatRegime=true) {onHeat=heatRegime;}


private:
    double alpha, T, tau1, tau2;
    bool onHeat;

};

#endif // THERMALEXPANSIONPOINT_H
