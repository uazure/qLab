#ifndef DILATOMETERDATA_H
#define DILATOMETERDATA_H

#include <QObject>
#include "qwt_series_data.h"
#include "thermalexpansionpoint.h"

class QFile;

class DilatometerData : public QObject, public QwtSeriesData<QPointF>
{
Q_OBJECT
public:
    explicit DilatometerData(QObject *parent = 0);

    inline QPointF sample(size_t i) const {return thermalExpansionVector.at(i);}
    inline size_t size() const {return thermalExpansionVector.size();}
    QRectF boundingRect() const {return qwtBoundingRect(*this);}

    void deletePoint(int index);

    //function that appends point with thermal expansion information
    bool setThermalExpansion(double T0, double T1, double dF, double Favg,double tau1=0,double tau2=0);
    void setL0(double L0);

    void setLog(const QString& log) {calculationLog=log;}
    const QString& getLog(void) {return calculationLog;}

    void saveToFile(QFile &file);


signals:

public slots:


private:
    //vector thermal expansion points
    //T() - is synonym for x() and alpha() - is synonym for y();
    QVector<ThermalExpansionPoint> thermalExpansionVector;

    //length of the sample
    double L0;

    /** This function returns sensitivity coefficient
      alpha = dL/dT * L0;
      sensitivity is used to calculate dL:
      dL = dF/sensitivity(Favg)
      where Favg = (F0+F1)/2
      */
    static double sensitivity (double frequency) {
        /*--------------------------------------------------------------*
           TableCurve Function: C:\\RABOTA\\C60-Pure-11-2005\\Calibr\\24_11_2005_lin.c Nov 24, 2005 5:30:45 PM
           C:\\RABOTA\\C60-Pure-11-2005\\Calibr\\24_11.txt
           X=
           Y=
           Eqn# 1  y=a+bx
           r2=0.8167947508843082
           r2adj=0.7923673843355493
           StdErr=211442614.3213579
           Fstat=71.33374222207029
           a= 24230920245.51454
           b= -945.9756196586279
         *--------------------------------------------------------------*/
        if (frequency>2.528104E+07) frequency=2.528104E+07;
        return 24230920245.51454+frequency*-945.9756196586279;
    }

    QString calculationLog;

};

#endif // DILATOMETERDATA_H
