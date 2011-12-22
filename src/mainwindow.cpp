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
    qDebug()<<outList;
    QString tmp;
    // clear device list
    devices.clear();
    for (int i=0, tmpint=0;i<outList.size();i++) {
        tmp=outList.at(i);
        // append string section before '(' as device
        // String is expected in format device(channel1;[channel2;])
        devices.append(tmp.left(tmp.indexOf('(')));

        // extract arguments specified in brackets
        tmp=tmp.mid(tmp.indexOf('(')+1,tmp.indexOf(')')-tmp.indexOf('(')-1);
    }
}
