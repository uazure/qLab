#ifndef THERMALEXPANSIONPOINT_H
#define THERMALEXPANSIONPOINT_H
#include <QPointF>

/** Class for storing data for single thermal expansion point
  Array of such points can be stored to allow direct access of QwtPlotCurve
  to double* data
  TODO: implement operator < and == to make it possible to sort array of
      ThermalExpansionPoint for adequate
  */
class ThermalExpansionPoint:public QPointF
{
public:
    ThermalExpansionPoint();
    ThermalExpansionPoint(double alpha, double T, double tau1=0, double tau2=0,bool onHeat=true);

    inline double getT() const {return this->x();}
    inline double getAlpha() const {return this->y();}
    inline double getTau1() const {return tau1;}
    inline double getTau2() const {return tau2;}
    inline bool isOnHeat() const {return onHeat;}
    inline void setT(double temp) {setX(temp);}
    inline void setAlpha(double a) {setY(a);}
    inline void setTau1(double t1) {tau1=t1;}
    inline void setTau2(double t2) {tau2=t2;}
    inline void setOnHeat (bool heatRegime=true) {onHeat=heatRegime;}


private:
    double tau1, tau2;
    bool onHeat;

};

#endif // THERMALEXPANSIONPOINT_H
