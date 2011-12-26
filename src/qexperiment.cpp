#include "qexperiment.h"

QExperiment::QExperiment(QString name, QObject *parent) :
    QObject(parent)
{
    this->name=name;
    settings=new QSettings (QSettings::IniFormat,QSettings::UserScope,QApplication::organizationName(),"experiment",this);
    timer=new QTimer(this);
    timer->setInterval(2000);
    timer->connect (timer,SIGNAL(timeout()),this,SLOT(doMeasure()));
}

QExperiment::~QExperiment() {
    delete settings;
    delete timer;
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
        deviceList[i]->setFactor(settings->value(name+"/"+deviceName+"/factor",0).toDouble());
        deviceList[i]->setMinValue(settings->value(name+"/"+deviceName+"/min_value",0).toDouble());
        deviceList[i]->setMaxValue(settings->value(name+"/"+deviceName+"/max_value",100).toDouble());
        deviceList[i]->setScaleHint(settings->value(name+"/"+deviceName+"/scale_hint",5).toDouble());
        deviceList[i]->setUnit(settings->value(name+"/"+deviceName+"/unit","Unit").toString());
        deviceList[i]->setLabel(settings->value(name+"/"+deviceName+"/label","").toString());
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
    qDebug("MEASURE STARTED");
    QByteArray tmp, output;
    for (int i=0;i<deviceList.size() && i<parametersList.size();i++) {
        deviceList.at(i)->readValue(tmp,parametersList.at(i));
        qDebug()<<tmp;
        output.append(tmp).append("\t");
    }
    emit measured(output.trimmed());
}
