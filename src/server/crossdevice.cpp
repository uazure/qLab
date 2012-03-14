#include "crossdevice.h"

CrossDevice::CrossDevice(Experiment *exp) :
        AbstractDevice(exp)
{
    shortname="cross";
    dev1=NULL;
    dev2=NULL;

}



bool CrossDevice::readValue(QByteArray &returnValue, QStringList parameters) {

    if (parameters.size()<4) {
        returnValue="-0.000";
        qWarning()<<"Failed to calculate crossfade value, not enough arguments. Should be at least 4 arguments: device1, device2, start, stop";
        return false;
    }

    if (dev1==NULL || dev2==NULL) {
        qDebug()<<"Initializing abstract devices for cross fade";
        dev1=experiment->findDevice(parameters.at(0));
        if (dev1==NULL) {
            qWarning()<<"Failed to find first device for crossfade:"<<parameters.at(0);
            returnValue="-0.000";
            return false;
        }

        dev2=experiment->findDevice(parameters.at(1));
        if (dev2==NULL) {
            qWarning()<<"Failed to find second device for crossfade:"<<parameters.at(1);
            returnValue="-0.000";
            return false;
        }
    }


    double val1=dev1->getLastValue().toDouble();
    double val2=dev2->getLastValue().toDouble();
    double start=parameters.at(2).toDouble();
    double end=parameters.at(3).toDouble();

    if (start>=end) {
        qWarning()<<"Start of crossfade larger than end";
        returnValue="-0.000";
        return false;
    }

    if (abs(val2-val1)>abs(end-start)) {
        qWarning()<<"Values of crossfade parameters differs more than start and end. Calculated value will be incorrect. Returning val1";
        returnValue=dev1->getLastValue();
        return false;
    }

    double result;
    if (val1<start) {
        result=val1;
    } else if (val2>end) {
        result=val2;
    } else if (val1>start && val1<end) {
        double weight2=(val1-start)/(end-start);
        double weight1=1-weight2;
        result=val1*weight1+val2*weight2;
    }

    returnValue=QByteArray::number(result,'g',8);
    lastValue=returnValue;
    return true;
}
