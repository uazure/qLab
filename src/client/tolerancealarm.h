#ifndef TOLERANCEALARM_H
#define TOLERANCEALARM_H

#include <QObject>

class ToleranceAlarm : public QObject
{
        Q_OBJECT
public:
    explicit ToleranceAlarm(double target=0,double tolerance=0,QWidget *parent=NULL);
    double target() const {return targetValue;}
    double tolerance(void) const {return toleranceValue;}
    bool isEnabled(void) const {return enabled;}

public slots:
    bool testCurrentValue(double value);
    void setTarget(double target) {targetValue=target;}
    void setTolerance(double tolerance) {toleranceValue=tolerance;}
    void disable() {enabled=false;}
    void enable() {enabled=true;}



private:
    double targetValue;
    double toleranceValue;
    QWidget *parentWidget;
    bool enabled;

};

#endif // TOLERANCEALARM_H
