#include "experimentconfigurationselectdialog.h"
#include "ui_experimentconfigurationselectdialog.h"

ExperimentConfigurationSelectDialog::ExperimentConfigurationSelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExperimentConfigurationSelectDialog)
{
    ui->setupUi(this);
    qDebug("ExperimentConfigurationSelectDialog created");
}

ExperimentConfigurationSelectDialog::~ExperimentConfigurationSelectDialog()
{
    qDebug("ExperimentConfigurationSelectDialog deleted");
    delete ui;
}

void ExperimentConfigurationSelectDialog::changeEvent(QEvent *e)
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

void ExperimentConfigurationSelectDialog::setExperimentList(QStringList &list) {
    ui->listWidget->insertItems(0,list);
    ui->listWidget->setCurrentRow(0);
}

void ExperimentConfigurationSelectDialog::accept() {
    QString value=ui->listWidget->currentIndex().data(Qt::DisplayRole).toString();
    emit experimentSelected(value);
    hide();
    close();
    delete this;
}

void ExperimentConfigurationSelectDialog::reject() {
    emit rejected();
    hide();
    close();
    delete this;
}
