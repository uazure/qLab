#include "experimentconfigurationselectdialog.h"
#include "ui_experimentconfigurationselectdialog.h"

ExperimentConfigurationSelectDialog::ExperimentConfigurationSelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExperimentConfigurationSelectDialog)
{
    ui->setupUi(this);
}

ExperimentConfigurationSelectDialog::~ExperimentConfigurationSelectDialog()
{
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
