#include "experimentcontroldialog.h"
#include "ui_experimentcontroldialog.h"

ExperimentControlDialog::ExperimentControlDialog(Experiment *experimentPointer, TcpClient *tcpClientPointer, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExperimentControlDialog)
{
    ui->setupUi(this);
    experiment=experimentPointer;
    tcpClient=tcpClientPointer;

    //set larger font for targetLabel
    QFont font=QWidget::font();
    //enlarge font for 30%
    font.setPointSizeF(font.pointSizeF()*1.3);
    font.setBold(true);
    ui->targetValueLabel->setFont(font);
    QStringList ctrl=experiment->getControl();
    ui->controlListWidget->addItems(ctrl);


    connect(ui->controlListWidget,SIGNAL(currentRowChanged(int)),SLOT(controlIndexChanged(int)));
    connect(tcpClient,SIGNAL(serverControlTarget(int,QString)),SLOT(setControlTarget(int,QString)));
    connect(ui->targetValueLabel,SIGNAL(doubleClicked()),SLOT(setControlTarget()));
}

ExperimentControlDialog::~ExperimentControlDialog()
{
    delete ui;
}

void ExperimentControlDialog::changeEvent(QEvent *e)
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

void ExperimentControlDialog::controlIndexChanged(int index) {
    ui->targetValueLabel->setText("...");
    tcpClient->query(TcpClient::queryTarget,QString::number(index));
}

void ExperimentControlDialog::setControlTarget(int control, QString value) {
    if (control!=ui->controlListWidget->currentRow()) {
        qWarning()<<"Can't update target value. Different control is selected.";
        return;
    }
    ui->targetValueLabel->setText(value);
}

/** this slot is used to ask user new target value for currently selected control
  */
void ExperimentControlDialog::setControlTarget() {
    int controlIndex=ui->controlListWidget->currentRow();
    if (controlIndex<0) {
        return;
    }

    QString currentTarget=ui->targetValueLabel->text();
    bool ok=false;
    double newTarget=QInputDialog::getDouble(this,tr("New target"),tr("Specify new target"),currentTarget.toDouble(),-2147483647,2147483647,2,&ok);
    if (!ok) {
        return;
    }
    qDebug()<<"Setting new target"<<newTarget<<"for control"<<controlIndex;
    tcpClient->setControlTarget(controlIndex,QString::number(newTarget));
    controlIndexChanged(controlIndex);
}
