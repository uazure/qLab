#include "experiment.h"

Experiment::Experiment(QString name, QObject *parent) :
    QObject(parent)
{
    this->name=name;
    settings=new QSettings (QSettings::IniFormat,QSettings::UserScope,QApplication::organizationName(),"experiment",this);
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
    dataStringList.clear();
    if (! name.isEmpty()) {
        startedOn=QDateTime().currentDateTime();
    }
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
    setFileName(startedOn.toString("yyyy-MM-dd ")+name+".dat");
    getHeader();
    //doMeasure();
    start();
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
        qDebug()<<"Initialized device"<<deviceList.at(i)->shortName();
        deviceList[i]->setFactor(settings->value(name+"/"+deviceNameWithParameters+"/factor",deviceList.at(i)->getFactor()).toDouble());
        deviceList[i]->setMinValue(settings->value(name+"/"+deviceNameWithParameters+"/min_value",deviceList.at(i)->getMinValue()).toDouble());
        deviceList[i]->setMaxValue(settings->value(name+"/"+deviceNameWithParameters+"/max_value",deviceList.at(i)->getMaxValue()).toDouble());
        deviceList[i]->setScaleHint(settings->value(name+"/"+deviceNameWithParameters+"/scale_hint",deviceList.at(i)->getScaleHint()).toDouble());
        deviceList[i]->setUnit(settings->value(name+"/"+deviceNameWithParameters+"/unit",deviceList.at(i)->getUnit()).toString());
        deviceList[i]->setLabel(settings->value(name+"/"+deviceNameWithParameters+"/label",deviceList.at(i)->getLabel()).toString());
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
    returnValue+="#Devices used: (name, label, min, max, unit)\n";
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

    returnValue+='\n';
    //qDebug()<<returnValue;
    return returnValue;
}

QString Experiment::getData() const {
    return this->dataStringList.join("\n");
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
