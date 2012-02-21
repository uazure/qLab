#include "experiment.h"

Experiment::Experiment(QString name, QObject *parent) :
    QObject(parent),
    ControllableDeviceList()
{
    this->name=name;
    settings=new QSettings (QSettings::IniFormat,QSettings::UserScope,QApplication::organizationName(),"experiment",this);
    settings->beginGroup(name);
    timer.connect (&timer,SIGNAL(timeout()),this,SLOT(doMeasure()));
    currentFileName="test.dat";
    saveTimer.setInterval(10000);
    saveTimer.connect(&saveTimer,SIGNAL(timeout()),this,SLOT(saveFile()));
    timer_progress=0;
    progressTimer.connect(&progressTimer,SIGNAL(timeout()),this,SLOT(updateProgress()));

}

Experiment::~Experiment() {
    delete settings;
    for (int i=0;i<deviceList.size();i++) {
        delete deviceList.at(i);
    }
}


void Experiment::setExperiment(QString experiment) {
    stop();
    setName(experiment);

    //set settings object to read from new experiment name section
    settings->endGroup();
    settings->beginGroup(name);

    dataStringList.clear();
    if (! name.isEmpty()) {
        startedOn=QDateTime().currentDateTime();
    }
    QString out=settings->value("out").toString();
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

    //initialize devices
    initDevices();

    //init controllers
    initControllers();

    // Define axis hint from config file (if available)
    axisStringList=settings->value("axis").toString().split(',',QString::KeepEmptyParts);

    //trim each entry
    for (int i=0;i<axisStringList.size();i++) {
        axisStringList[i]=axisStringList.at(i).trimmed();
    }
    setFileName(startedOn.toString("yyyy-MM-dd ")+name+".dat");

    start();
}

void Experiment::initControllers() {
    //read controllers from experiment settings
    QStringList controls=settings->value("controls").toString().split(",",QString::SkipEmptyParts);

    //Clean the controlers
    ControllableDeviceList::clear();

    //Go thru controls list and initiate them
    for (int i=0;i<controls.size();i++) {
        QString deviceNameAndLoopName=controls.at(i).trimmed();
        //interpret string befor '.' as device name
        QString deviceName=deviceNameAndLoopName.section('.',0,0);
        //and string after dot as channel name
        QString loopName=deviceNameAndLoopName.section('.',-1,-1);

        //set device as pointer to AbstractDevice
        AbstractDevice* device=findDevice(deviceName);

        //continue if no device found
        if (device==NULL) {
            continue;
        }

        qDebug()<<"Converting AbstractDevice"<<deviceName<<"to AbstractThermocontrollerDevice";
        AbstractThermocontrollerDevice *thermocontrollerDevice =
                dynamic_cast<AbstractThermocontrollerDevice *> (device);

        int loopIndex=thermocontrollerDevice->getLoopIndex(loopName);
        qDebug()<<"Appending control:"<<deviceName<<"loop name"<<loopName<<"loop index"<<loopIndex;
        appendControl(thermocontrollerDevice,loopIndex);
    }
}

AbstractDevice * Experiment::findDevice(QString deviceName) const {
    for (int i=0;i<deviceList.size();++i) {
        if (deviceList.at(i)->shortName()==deviceName) {
            return deviceList.at(i);
        }
    }
    qWarning()<<"Failed to find device"<<deviceName;
    return NULL;
}

void Experiment::initDevices() {
    // delete existing QAbstractDevice child objects from list:
    for (int i=0;i<deviceList.size();i++) {
        delete deviceList.at(i);
    }
    // clear the device list
    deviceList.clear();

    // go thru all devices we need to init and init them!
    for (int i=0;i<deviceStringList.size() && i<parametersList.size();i++) {
        QByteArray deviceName=deviceStringList.at(i).toUtf8();
        QByteArray deviceNameWithParameters=deviceName;
        if (! parametersList.at(i).isEmpty() ) {
            deviceNameWithParameters.append('(');
            for (int j=0;j<parametersList.at(i).size();j++) {
                deviceNameWithParameters.append(parametersList.at(i).at(j).toUtf8()+';');
            }
            //replace last character (i.e. ';') with ')';
            deviceNameWithParameters[deviceNameWithParameters.size()-1]=')';
            qDebug()<<"deviceNameWithParameters"<<deviceNameWithParameters;
        }
        deviceList.append(DeviceFarm::getDeviceObject(deviceName));
//        if (deviceList[i]->capable("thermocontroller")) {
//            //FIXME: controlList should ask for controlled channels from device
//            appendControl(dynamic_cast <AbstractThermocontrollerDevice *> (deviceList[i]),0);
//        }

        qDebug()<<"Initialized device"<<deviceList.at(i)->shortName();
        deviceList[i]->setFactor(settings->value(deviceNameWithParameters+"/factor",deviceList.at(i)->getFactor()).toDouble());
        deviceList[i]->setMinValue(settings->value(deviceNameWithParameters+"/min_value",deviceList.at(i)->getMinValue()).toDouble());
        deviceList[i]->setMaxValue(settings->value(deviceNameWithParameters+"/max_value",deviceList.at(i)->getMaxValue()).toDouble());
        deviceList[i]->setScaleHint(settings->value(deviceNameWithParameters+"/scale_hint",deviceList.at(i)->getScaleHint()).toDouble());
        deviceList[i]->setUnit(settings->value(deviceNameWithParameters+"/unit",deviceList.at(i)->getUnit()).toString());
        deviceList[i]->setLabel(settings->value(deviceNameWithParameters+"/label",deviceList.at(i)->getLabel()).toString());
    }
}

void Experiment::setName(QString name) {
    if (this->name!=name) {
        this->name=name;
        stop();
        dataStringList.clear();
        if (! name.isEmpty()) {
            emit experimentChanged(name);
        }
    }
}

QString Experiment::getName() const {
    return name;
}

void Experiment::doMeasure() {
    timer_progress=0;
    progressTimer.start();;
    emit updateProgress(100);
    //qDebug("MEASURE CYCLE START");
    QByteArray tmp, output;
    for (int i=0;i<deviceList.size() && i<parametersList.size();i++) {
        deviceList.at(i)->readValue(tmp,parametersList.at(i));
        //qDebug()<<tmp;
        output.append(tmp).append("\t");
    }
    //qDebug("MEASURE CYCLE END");
    this->dataStringList.append(output.trimmed());
    emit measured(output.trimmed());
}

void Experiment::setInterval(int msec) {
    if (msec<min_interval) {
        qWarning()<<"Will not set interval to"<<msec<<"because it's too low";

        emit TcpForbidden("Interval is too low. Accepted values from "+
                          QString::number(min_interval)+" to "+
                          QString::number(max_interval)+" (msec)");
        return;
    }
    if (msec>max_interval) {
        qWarning()<<"Will not set interval to"<<msec<<"because it's too high";
        emit TcpForbidden("Interval is too high. Accepted values from "+
                          QString::number(min_interval)+" to "+
                          QString::number(max_interval)+" (msec)");
        return;
    }

    if (msec!=timer.interval()) {
        timer_progress=0;
        timer.setInterval(msec);
        progressTimer.setInterval(msec/10);
        emit intervalChanged(msec);
    }
}

bool Experiment::isActive() const {
    return timer.isActive();
}

void Experiment::start() {
    bool status=isActive();
    if (! name.isEmpty()) {
        timer.start();
        saveTimer.start();
        progressTimer.start();

    } else {
        qWarning("Experiment name is not set");
    }
    if (status!=isActive()) {
        emit statusChanged(isActive());
        emit Notify("Started");
    }
}

void Experiment::start(bool arg) {
    if (arg) start();
    else stop();
}

void Experiment::stop() {
    bool status=isActive();
    timer.stop();
    saveTimer.stop();
    progressTimer.stop();
    timer_progress=0;
    if (status!=isActive()) {
    emit statusChanged(isActive());
    emit updateProgress(0);
    emit Notify("Stopped");
}
}

QString Experiment::getHeader() const {
    QString returnValue;

    returnValue+="#Experiment "+name+" started on "+startedOn.toString("dd.MM.yyyy hh:mm (dddd)")+"\n";
    returnValue+="#Device:\n#";
    for (int i=0;i<deviceList.size();i++,returnValue+="\t") {
        returnValue+=deviceList.at(i)->shortName();
    }
    returnValue+="\n#Label:\n#";
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
    returnValue+="\n#Min:\n#";
    for (int i=0;i<deviceList.size();i++,returnValue+="\t") {
        returnValue+=QString::number(deviceList.at(i)->getMinValue());
    }
    returnValue+="\n#Max:\n#";
    for (int i=0;i<deviceList.size();i++,returnValue+="\t") {
        returnValue+=QString::number(deviceList.at(i)->getMaxValue());
    }
    returnValue+="\n#Unit:\n#";
    for (int i=0;i<deviceList.size();i++,returnValue+="\t") {
        returnValue+=deviceList.at(i)->getUnit();
    }
    returnValue+="\n#Axis hint:\n#";

    for (int i=0;i<deviceList.size();i++,returnValue+="\t") {
        if(deviceList.at(i)->shortName()=="utime") {
            returnValue+="xBottom";
        } else if (i<axisStringList.size()){
            returnValue+=axisStringList.at(i);
        } else {
            returnValue+="yLeft"; //other variant - to use "none"
        }
    }

    returnValue+='\n';
    //qDebug()<<returnValue;
    return returnValue;
}

QString Experiment::getData() const {
    return dataStringList.join("\n");
}
QStringList Experiment::getDataStringList() const {
    return dataStringList;
}

void Experiment::saveFile() {
    if (currentFileName.isEmpty()) {
        qWarning()<<"No file specified. Nothing to save";
        return;
    }

    QFile file(currentFileName);
    if (!file.open(QFile::WriteOnly))
    {
        qWarning()<<"Failed to open file"<<currentFileName<<"for writing";
        return;
    }
    QTextStream stream(&file);
    stream<<getHeader();
    for (int i=0;i<dataStringList.size();i++) {
        stream<<dataStringList.at(i)+'\n';
    }
    file.close();
    qDebug()<<"File saved ok:"<<currentFileName;
    emit Notify("File saved Ok");
}

void Experiment::setFileName(QString filename) {
    // do nothing is string is NULL (i.e. no file was selected);
    if (filename.isNull()) return;
    if (filename!=currentFileName) {
        currentFileName=filename;
        emit fileChanged(filename);
        emit Notify("Filename: "+filename);
    }
}

QString Experiment::getCurrentFileName() const {
    return currentFileName;
}

int Experiment::getInterval() const {
    return timer.interval();
}

void Experiment::updateProgress(void) {
    if (!isActive()) return;
    //qDebug()<<"progressTimer"<<progressTimer.interval();
    //qDebug()<<"timer_progress"<<timer_progress;
    timer_progress=timer_progress+progressTimer.interval();
    int progress=timer_progress*100/timer.interval();
    //qWarning()<<"update progress issued"<<progress;
    if (progress>100) qWarning()<<"Progress:"<<progress;
    else
        emit updateProgress(progress);
}

void Experiment::setTarget(QString value, int control)
{
    //defining device pointer of the requested control
    AbstractThermocontrollerDevice *device=ControllableDeviceList::getControlDevice(control);
    if (device==NULL) {
        qWarning()<<"Failed to get control"<<control;
        return;
    }

    //defining channel index of the requested control
    int loopIndex=ControllableDeviceList::getControlLoopIndex(control);

    //setting target of the thermocontroller loop with index deviceChannelIndex
    qDebug()<<"Setting target of"<<device<<"with loop index"<<loopIndex<< "to"<<value;
    device->setTargetValue(value,loopIndex);
}
