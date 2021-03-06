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

//    if (ctrl.size()>0) {
//        ui->controlListWidget->setCurrentRow(0);
//    }


    connect(ui->controlListWidget,SIGNAL(currentRowChanged(int)),SLOT(controlIndexChanged(int)));
    connect(ui->targetValueLabel,SIGNAL(doubleClicked()),SLOT(setControlTarget()));

    connect(tcpClient,SIGNAL(serverControlTarget(int,QString)),SLOT(setControlTarget(int,QString)));
    connect(tcpClient,SIGNAL(serverHistory(QStringList)),SLOT(setHistory(QStringList)));
}

ExperimentControlDialog::~ExperimentControlDialog()
{
    delete ui;
    qDebug()<<"ExperimentControlDialog deleted";
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
    tcpClient->query(TcpClient::queryHistory);
}

void ExperimentControlDialog::setControlTarget(int control, QString value) {
    int currentRow=ui->controlListWidget->currentRow();
    if (control!=currentRow) {
        qWarning()<<"Current row is"<<currentRow<<"Different control is selected:"<<control;
        return;
    }
    ui->targetValueLabel->setText(value);
}

/** this slot is used to ask user new target value for currently selected control
  */
void ExperimentControlDialog::setControlTarget() {
    const int controlIndex=ui->controlListWidget->currentRow();
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

void ExperimentControlDialog::setHistory(QStringList history) {
    ui->historyListWidget->clear();
    if (history.isEmpty()) {
        return;
    }
    const int controlIndex=ui->controlListWidget->currentRow();
    QStringList controlHistory;
    for (int i=0;i<history.size();++i) {
        QStringList cols=history.at(i).split("\t");
        if (cols.at(0).isEmpty() || cols.at(1).isEmpty()) {
            continue;
        }
        bool ok=false;
        if (controlIndex==cols.at(0).toInt(&ok) && ok) {
            controlHistory.append(cols.at(1));
        }
    }

    //reverse stringList
    QStringList reversedHistory;
    for (int i=controlHistory.size()-1;i>=0;--i) {
        reversedHistory.append(controlHistory.at(i));
    }

    //update history widget with history items
    ui->historyListWidget->addItems(reversedHistory);
}
