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
    QString target=experiment->getControlTarget(index);
    ui->targetValueLabel->setText(target);
}
