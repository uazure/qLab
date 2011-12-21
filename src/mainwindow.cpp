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
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionFullscreen,SIGNAL(toggled(bool)),this,SLOT(setFullscreen(bool)));
    connect(ui->actionAbout_Qt,SIGNAL(triggered()),this,SLOT(showAboutQt()));



    QByteArray shortname="k2000";

    QAbstractDevice *dev = DeviceFarm::getDeviceObject(shortname);
    QByteArray arr;
    dev->readValue(arr);
    ui->label->setText(arr);
    delete dev;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setFullscreen(bool a) {
    if (a)
     this->setWindowState(Qt::WindowFullScreen);
    else
     this->setWindowState(Qt::WindowNoState);
}

void MainWindow::showAboutQt(void) {
    QApplication::aboutQt();
}
