#include "controlmanagementdialog.h"
#include "ui_controlmanagementdialog.h"

ControlManagementDialog::ControlManagementDialog(Experiment *experiment, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ControlManagementDialog)
{
    ui->setupUi(this);
    this->experiment=experiment;
    ui->listWidget->addItems(experiment->getControlList());
    if (ui->listWidget->count()) {
        ui->groupBox->setEnabled(true);
    }
    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),SLOT(updateControlInfo(int)));
}

ControlManagementDialog::~ControlManagementDialog()
{
    delete ui;
}

void ControlManagementDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void ControlManagementDialog::updateControlInfo(int index) {
    AbstractThermocontrollerDevice *tcdevice=experiment->getControlDevice(index);
    AbstractDevice *device=dynamic_cast<AbstractDevice *>(tcdevice);
    int loopIndex=experiment->getControlLoopIndex(index);
    QString value=tcdevice->getTargetValue(loopIndex);
    QString loopName=tcdevice->getLoopName(loopIndex);
    QString channel=tcdevice->getControlChannel(loopIndex);

    ui->targetLabel->setText(value);
    ui->loopNameLabel->setText(loopName);
    ui->loopNameLabel->setToolTip(QString::number(loopIndex));
    ui->channelLabel->setText(channel);
}

void ControlManagementDialog::changeTarget(int controlIndex) {
    //FIXME
}
