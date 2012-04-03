#ifndef DILATOMETERDATA_H
#define DILATOMETERDATA_H

#include <QObject>
#include "qwt_series_data.h"
#include "thermalexpansionpoint.h"


class DilatometerData : public QObject, public QwtSeriesData<QPointF>
{
Q_OBJECT
public:
    explicit DilatometerData(QObject *parent = 0);
    QVector<ThermalExpansionPoint> thermalExpansionVector;
    inline QPointF sample(size_t i) const {return thermalExpansionVector.at(i);}
    inline size_t size() const {return thermalExpansionVector.size();}
    QRectF boundingRect() const {return qwtBoundingRect(*this);}





signals:

public slots:


private:

    //length of the sample
    double L0;

    /** This function returns sensitivity coefficient
      alpha = dL/dT * L0;
      sensitivity is used to calculate dL:
      dL = dF/sensitivity(Favg)
      where Favg = (F0+F1)/2
      */
    double sensitivity (double frequency) const {
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

};

#endif // DILATOMETERDATA_H
