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
    QLocale::setDefault(QLocale::English);
    ui->setupUi(this);
    experimentSettings=new QSettings (QSettings::IniFormat,QSettings::UserScope,QApplication::organizationName(),"experiment",this);
    experiment=new QExperiment();
    tcp=new TcpServer(this);

    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionFullscreen,SIGNAL(toggled(bool)),this,SLOT(setFullscreen(bool)));
    connect(ui->actionAbout_Qt,SIGNAL(triggered()),this,SLOT(showAboutQt()));
    connect(ui->actionNew_experiment,SIGNAL(triggered()),this,SLOT(getExperiment()));
    connect(ui->actionSave,SIGNAL(triggered()),experiment,SLOT(saveFile()));
    connect(ui->actionSave_as,SIGNAL(triggered()),this,SLOT(getFile()));
    connect(ui->actionStartMeasure,SIGNAL(toggled(bool)),experiment,SLOT(start(bool)));

    connect(experiment,SIGNAL(statusChanged(bool)),ui->actionStartMeasure,SLOT(setChecked(bool)));
    connect(experiment,SIGNAL(statusChanged(bool)),this,SLOT(statusChanged(bool)));
    connect(experiment,SIGNAL(experimentChanged(QString)),this,SLOT(setExperiment(QString)));
    connect(experiment,SIGNAL(measured(QString)),ui->plainTextEdit,SLOT(appendPlainText(QString)));
    connect(experiment,SIGNAL(fileChanged(QString)),this,SLOT(setFile(QString)));
    connect(experiment,SIGNAL(Notify(QString)),SLOT(Notify(QString)));
    connect(experiment,SIGNAL(intervalChanged(int)),SLOT(setMeasureInterval(int)));

    connect(tcp,SIGNAL(clientCountChanged(int)),&tcpClientsLabel,SLOT(setNum(int)));

    connect(ui->measureIntervalSpinBox,SIGNAL(valueChanged(int)),SLOT(setMeasureInterval(int)));



    experiment->setInterval(2000);

    // Additional ui preparation
    experimentLabel.setToolTip("Experiment configuration");
    statusLabel.setToolTip("Experiment status:\nR - running\nS - stopped");
    QFont font(statusLabel.font());
    font.setBold(true);
    statusLabel.setFont(font);
    //set experiment status to stopped
    statusChanged(false);
    tcpClientsLabel.setToolTip("Number of tcp clients");
    tcpClientsLabel.setNum(0);

    statusBar()->addPermanentWidget(&tcpClientsLabel);
    statusBar()->addPermanentWidget(&fileLabel);
    statusBar()->addPermanentWidget(&experimentLabel);
    statusBar()->addPermanentWidget(&statusLabel);

    // Ask user to select experiment
    getExperiment();
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
    dialog->connect (dialog,SIGNAL(experimentSelected(QString)),experiment,SLOT(setExperiment(QString)));
    dialog->show();
}

void MainWindow::setExperiment(QString  experimentName) {
    // update ui
    qDebug()<<"Experiment set to"<<experimentName;
    experimentLabel.setText(experimentName);
    statusBar()->showMessage("Experiment configuration: "+experimentName);
}

void MainWindow::getFile() {
     experiment->setFileName(
QFileDialog::getSaveFileName(this,tr("Save as..."), experiment->getCurrentFileName(), tr("Data files (*.dat *.txt)")));
     experiment->saveFile();
}

void MainWindow::setFile(QString filename) {
    QFileInfo fileinfo=QFileInfo(filename);
    fileLabel.setToolTip(filename);
    fileLabel.setText(fileinfo.fileName());
    this->statusBar()->showMessage("File: "+filename);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (event->spontaneous()) event->accept();

    ConfirmExitDialog *dialog=new ConfirmExitDialog(this);
    dialog->setQuestion("Really exit?");
    dialog->activateWindow();

    if (dialog->exec())
    {
        experiment->saveFile();
        event->accept();
    } else {
        event->ignore();
    }
    delete dialog;
}

void MainWindow::Notify(QString message) {
    statusBar()->showMessage(message);
}

void MainWindow::statusChanged(bool status) {
    if (status) {
        statusLabel.setText("R");
        statusLabel.setStyleSheet("QLabel {color: green;}");
    } else {
        statusLabel.setText("S");
        statusLabel.setStyleSheet("QLabel {background-color: red;color:white;}");
    }
}

void MainWindow::setMeasureInterval(int msec) {
    if (ui->measureIntervalSpinBox->value()!=msec)
    ui->measureIntervalSpinBox->setValue(msec);

    experiment->setInterval(msec);
}
