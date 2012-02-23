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
    connect(ui->targetLabel,SIGNAL(doubleClicked()),SLOT(changeTarget()));

    //set larger font for targetLabel
    QFont font=QWidget::font();
    //enlarge font for 30%
    font.setPointSizeF(font.pointSizeF()*1.3);
    font.setBold(true);
    ui->targetLabel->setFont(font);
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

void ControlManagementDialog::changeTarget() {
    int index=ui->listWidget->currentRow();
    AbstractThermocontrollerDevice *tcdevice=experiment->getControlDevice(index);
    int loopIndex=experiment->getControlLoopIndex(index);
    QString currentTarget=tcdevice->getTargetValue(loopIndex);
    double curTarget=currentTarget.toDouble();
    bool ok=false;
    double newTarget=QInputDialog::getDouble(this,tr("Set target"),tr("Set target value"),curTarget,-2147483647,2147483647,2,&ok);
    if (!ok) {
        qWarning("Target not set");
        return;
    }

    qDebug()<<"Setting target value of control"<<index<<"loop"<<loopIndex<<"to"<<newTarget;
    ok=tcdevice->setTargetValue(QString::number(newTarget),loopIndex);
    if (!ok) {
        QMessageBox::warning(this,"Value not set","Failed to set target value");
        qWarning()<<"Failed to setTargetValue";
    }
    ui->targetLabel->setText(tcdevice->getTargetValue(loopIndex));
}
