#include "tolerancealarm.h"
#include <cmath>
#include <QMessageBox>
#include <QString>
#include <QDebug>

ToleranceAlarm::ToleranceAlarm(double target, double tolerance, QWidget *parent):
        QObject(parent)
{
    this->parentWidget=parent;
    this->targetValue=target;
    if (tolerance==0) {
        //set 5% tolerance for target value
        tolerance=target*0.05;
    }
    toleranceValue=fabs(tolerance);
    enabled=true;
}

bool ToleranceAlarm::testCurrentValue(double value) {
    if (fabs(value-targetValue)>toleranceValue) {
        qWarning()<<"Value exceeds tolerance";
        return false;
        //
    }
    return true;
}
