#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSettings::setDefaultFormat(QSettings::IniFormat);
    connect(ui->ReadSettings,SIGNAL(clicked()),this,SLOT(readSettings()));
    connect(ui->WriteSettings,SIGNAL(clicked()),this,SLOT(writeSettings()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::readSettings(void) {
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings *settings;
    if (ui->ScopeSelector->currentIndex()==1) {
        settings=new QSettings(QSettings::IniFormat,QSettings::SystemScope,ui->OrganizationName->text(),ui->AppName->text(),this);
    } else {
        settings=new QSettings(QSettings::IniFormat,QSettings::UserScope,ui->OrganizationName->text(),ui->AppName->text(),this);

    }

    settings->setDefaultFormat(QSettings::IniFormat);
    ui->Value->setPlainText(settings->value(ui->Key->text()).toString());

    delete settings;
}

void MainWindow::writeSettings(void) {
    QSettings *settings;
    if (ui->ScopeSelector->currentIndex()==1) {
        settings=new QSettings(QSettings::IniFormat,QSettings::SystemScope,ui->OrganizationName->text(),ui->AppName->text(),this);
    } else {
        settings=new QSettings(QSettings::IniFormat,QSettings::UserScope,ui->OrganizationName->text(),ui->AppName->text(),this);

    }
    settings->setDefaultFormat(QSettings::IniFormat);
    settings->setValue(ui->Key->text(),QVariant::QVariant(ui->Value->toPlainText()));
    delete settings;
}
