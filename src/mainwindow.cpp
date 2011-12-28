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

    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionFullscreen,SIGNAL(toggled(bool)),this,SLOT(setFullscreen(bool)));
    connect(ui->actionAbout_Qt,SIGNAL(triggered()),this,SLOT(showAboutQt()));
    connect(ui->actionNew_experiment,SIGNAL(triggered()),this,SLOT(getExperiment()));
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(getFile()));
    connect(experiment,SIGNAL(experimentChanged(QString)),this,SLOT(setExperiment(QString)));
    connect(experiment,SIGNAL(measured(QString)),ui->plainTextEdit,SLOT(appendPlainText(QString)));
    connect(experiment,SIGNAL(fileChanged(QString)),this,SLOT(setFile(QString)));

    // Additional ui preparation
    experimentLabel.setToolTip("Experiment configuration");
    statusBar()->addPermanentWidget(&experimentLabel);

    // Ask user to select experiment
    this->getExperiment();

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
    this->statusBar()->showMessage("Experiment configuration: "+experimentName);
}

void MainWindow::getFile() {
     experiment->setFileName(
QFileDialog::getOpenFileName(this,tr("Open data file"), "", tr("Data files (*.dat *.txt)")));
}

void MainWindow::setFile(QString filename) {
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
        event->accept();
    } else {
        event->ignore();
    }
    delete dialog;
}
