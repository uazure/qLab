#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qgpibdevice.h"
#include <QDebug>
#include <QSystemTrayIcon>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    experimentSettings=new QSettings (QSettings::IniFormat,QSettings::UserScope,QApplication::organizationName(),"experiment",this);



    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionFullscreen,SIGNAL(toggled(bool)),this,SLOT(setFullscreen(bool)));
    connect(ui->actionAbout_Qt,SIGNAL(triggered()),this,SLOT(showAboutQt()));
    connect(ui->actionNew_experiment,SIGNAL(triggered()),this,SLOT(getExperiment()));

    // Additional ui preparation
    experimentLabel.setToolTip("Experiment configuration");
    statusBar()->addPermanentWidget(&experimentLabel);


    // Ask user to select experiment
    this->getExperiment();


//    QByteArray shortname="k2000";
//    QAbstractDevice *dev = DeviceFarm::getDeviceObject(shortname);
//    QByteArray arr;
//    dev->readValue(arr);
//    ui->label->setText(arr);
//    delete dev;
}

MainWindow::~MainWindow()
{
    delete experimentSettings;
    delete ui;
}

void MainWindow::setFullscreen(bool a) {
    if (a)
     setWindowState(Qt::WindowFullScreen);
    else
     setWindowState(windowState() ^ Qt::WindowFullScreen);
}



void MainWindow::showAboutQt(void) {
    QApplication::aboutQt();
}

void MainWindow::getExperiment() {
    /* We need to read all available sections from QSettings object for
    experiment types
       */
    QStringList experimentList=experimentSettings->childGroups();
    ExperimentConfigurationSelectDialog *dialog=new ExperimentConfigurationSelectDialog(this);
    dialog->setExperimentList(experimentList);
    dialog->connect (dialog,SIGNAL(experimentSelected(QString)),this,SLOT(setExperiment(QString)));
    dialog->show();
}

void MainWindow::setExperiment(QString  experiment) {
    // update ui
    this->experiment=experiment;
    qDebug()<<"Experiment set to"<<experiment;
    experimentLabel.setText(experiment);
    this->statusBar()->showMessage("Experiment configuration: "+experiment);

    // read configuration

    QString out=experimentSettings->value(experiment+"/out").toString();
    qDebug()<<"Output read from"<<experiment<<"config:"<<out;
    QStringList outList=out.split(",",QString::SkipEmptyParts);
    QString tmp;
    // clear device list
    devices.clear();
    parameters.clear();
    for (int i=0;i<outList.size();i++) {
        tmp=outList.at(i);
        tmp=tmp.trimmed();
        if (tmp.indexOf('(')==-1) {
            devices.append(tmp);
            parameters.append(QStringList());

        } else {
            devices.append(tmp.left(tmp.indexOf('(')).trimmed());
            // extract arguments specified in brackets separated with ';'
            tmp=tmp.mid(tmp.indexOf('(')+1,tmp.indexOf(')')-tmp.indexOf('(')-1).trimmed();
            QStringList tmpstringlist=tmp.split(';');
            for (int j=0;j<tmpstringlist.size();j++) {
                tmpstringlist[j]=tmpstringlist[j].trimmed();
            }
            parameters.append(tmpstringlist);
        }
    }
    qDebug()<<"Config read:\nDevices:"<<devices<<"\nParameters:"<<parameters;
    initDevices();
}

void MainWindow::initDevices() {
    // delete existing QAbstractDevice child objects from list:
    for (int i=0;i<deviceList.size();i++) {
        delete deviceList.at(i);
    }
    // clear the device list
    deviceList.clear();

    // go thru all devices we need to init and init them!
    for (int i=0;i<devices.size();i++) {
        QByteArray deviceName=devices.at(i).toAscii();
        deviceList.append(DeviceFarm::getDeviceObject(deviceName));
        qDebug()<<"Initialized device"<<deviceList.at(i)->shortName();
        deviceList[i]->setFactor(experimentSettings->value(experiment+"/"+deviceName+"/factor",0).toDouble());
        deviceList[i]->setMinValue(experimentSettings->value(experiment+"/"+deviceName+"/min_value",0).toDouble());
        deviceList[i]->setMaxValue(experimentSettings->value(experiment+"/"+deviceName+"/max_value",100).toDouble());
        deviceList[i]->setScaleHint(experimentSettings->value(experiment+"/"+deviceName+"/scale_hint",5).toDouble());
        deviceList[i]->setUnit(experimentSettings->value(experiment+"/"+deviceName+"/unit","Unit").toString());
        deviceList[i]->setLabel(experimentSettings->value(experiment+"/"+deviceName+"/label","").toString());
    }
}
