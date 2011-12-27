#include "qexperiment.h"

QExperiment::QExperiment(QString name, QObject *parent) :
    QObject(parent)
{
    startedOn=QDateTime::currentDateTime();
    this->name=name;
    settings=new QSettings (QSettings::IniFormat,QSettings::UserScope,QApplication::organizationName(),"experiment",this);
    timer=new QTimer(this);
    timer->setInterval(2000);
    timer->connect (timer,SIGNAL(timeout()),this,SLOT(doMeasure()));
}

QExperiment::~QExperiment() {
    delete settings;
    delete timer;
    for (int i=0;i<deviceList.size();i++) {
        delete deviceList.at(i);
    }
}

void QExperiment::setExperiment(QString experiment) {
    setName(experiment);
    QString out=settings->value(experiment+"/out").toString();
    qDebug()<<"Output read from"<<experiment<<"config:"<<out;
    QStringList outList=out.split(",",QString::SkipEmptyParts);
    QString tmp;
    // clear device list
    deviceStringList.clear();
    parametersList.clear();
    for (int i=0;i<outList.size();i++) {
        tmp=outList.at(i);
        tmp=tmp.trimmed();
        if (tmp.indexOf('(')==-1) {
            deviceStringList.append(tmp);
            parametersList.append(QStringList());

        } else {
            deviceStringList.append(tmp.left(tmp.indexOf('(')).trimmed());
            // extract arguments specified in brackets separated with ';'
            tmp=tmp.mid(tmp.indexOf('(')+1,tmp.indexOf(')')-tmp.indexOf('(')-1).trimmed();
            QStringList tmpstringlist=tmp.split(';');
            for (int j=0;j<tmpstringlist.size();j++) {
                tmpstringlist[j]=tmpstringlist[j].trimmed();
            }
            parametersList.append(tmpstringlist);
        }
    }
    qDebug()<<"Config read:\nDevices:"<<deviceStringList<<"\nParameters:"<<parametersList;
    initDevices();
    getHeader();
    doMeasure();
    timer->start();
}

void QExperiment::initDevices() {
    // delete existing QAbstractDevice child objects from list:
    for (int i=0;i<deviceList.size();i++) {
        delete deviceList.at(i);
    }
    // clear the device list
    deviceList.clear();

    // go thru all devices we need to init and init them!
    for (int i=0;i<deviceStringList.size();i++) {
        QByteArray deviceName=deviceStringList.at(i).toAscii();
        deviceList.append(DeviceFarm::getDeviceObject(deviceName));
        qDebug()<<"Initialized device"<<deviceList.at(i)->shortName();
        deviceList[i]->setFactor(settings->value(name+"/"+deviceName+"/factor",deviceList.at(i)->getFactor()).toDouble());
        deviceList[i]->setMinValue(settings->value(name+"/"+deviceName+"/min_value",deviceList.at(i)->getMinValue()).toDouble());
        deviceList[i]->setMaxValue(settings->value(name+"/"+deviceName+"/max_value",deviceList.at(i)->getMaxValue()).toDouble());
        deviceList[i]->setScaleHint(settings->value(name+"/"+deviceName+"/scale_hint",deviceList.at(i)->getScaleHint()).toDouble());
        deviceList[i]->setUnit(settings->value(name+"/"+deviceName+"/unit",deviceList.at(i)->getUnit()).toString());
        deviceList[i]->setLabel(settings->value(name+"/"+deviceName+"/label",deviceList.at(i)->getLabel()).toString());
    }
}

void QExperiment::setName(QString name) {
    this->name=name;
    emit experimentChanged(name);
}

QString QExperiment::getName() const {
    return name;
}

void QExperiment::doMeasure() {
    qDebug("MEASURE CYCLE START");
    QByteArray tmp, output;
    for (int i=0;i<deviceList.size() && i<parametersList.size();i++) {
        deviceList.at(i)->readValue(tmp,parametersList.at(i));
        qDebug()<<tmp;
        output.append(tmp).append("\t");
    }
    qDebug("MEASURE CYCLE END");
    emit measured(output.trimmed());
}

void QExperiment::setInterval(int msec) {
    timer->setInterval(msec);
    emit intervalChanged(msec);
}

bool QExperiment::isActive() const {
    return timer->isActive();
}

void QExperiment::start() {
    timer->start();
    emit statusChanged(isActive());
}

void QExperiment::stop() {
    timer->stop();
    emit statusChanged(isActive());
}

QString QExperiment::getHeader() {
    QString returnValue;

    returnValue+="#Experiment "+name+" started on "+startedOn.toString("dd.MM.yyyy hh:mm (dddd)")+"\n";
    returnValue+="#Devices used: (name, label, min, max, unit)\n";
//    for (int i=0;i<deviceList.size();i++) {
//        returnValue+='#'+deviceList.at(i)->shortName()+' '+
//                     deviceList.at(i)->getLabel()+" ("+
//                     QString::number(deviceList.at(i)->getMinValue())+" - "+
//                     QString::number(deviceList.at(i)->getMaxValue())+" "+
//                     deviceList.at(i)->getUnit()+") min scale: "+
//                     QString::number(deviceList.at(i)->getScaleHint())+"\n";
//    }
    returnValue+="#";
    for (int i=0;i<deviceList.size();i++,returnValue+="\t") {
        returnValue+=deviceList.at(i)->shortName();
    }
    returnValue+="\n#";
    for (int i=0;i<deviceList.size();i++,returnValue+="\t") {
        returnValue+=deviceList.at(i)->getLabel();
    }
    /* Uncomment this if you need factor printed in header of the experiment datafile.
    returnValue+="\n#";
    for (int i=0;i<deviceList.size();i++,returnValue+="\t") {
        if (deviceList.at(i)->getFactor()==0 || deviceList.at(i)->getFactor()==1) {
            returnValue+="y";
        } else {
            returnValue+='y*'+QString::number(deviceList.at(i)->getFactor());
        }
    }
    */
    returnValue+="\n#";
    for (int i=0;i<deviceList.size();i++,returnValue+="\t") {
        returnValue+=QString::number(deviceList.at(i)->getMinValue());
    }
    returnValue+="\n#";
    for (int i=0;i<deviceList.size();i++,returnValue+="\t") {
        returnValue+=QString::number(deviceList.at(i)->getMaxValue());
    }
    returnValue+="\n#";
    for (int i=0;i<deviceList.size();i++,returnValue+="\t") {
        returnValue+=deviceList.at(i)->getUnit();
    }


    emit measured(returnValue);
    return returnValue;
}
