#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qgpibdevice.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QGpibDevice *dev = new QGpibDevice(14,this);
    QByteArray arr;
    dev->readValue(0,&arr);
    ui->label->setText(arr);
    delete dev;


}

MainWindow::~MainWindow()
{
    delete ui;
}
