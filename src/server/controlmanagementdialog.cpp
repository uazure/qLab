#include "controlmanagementdialog.h"
#include "ui_controlmanagementdialog.h"

ControlManagementDialog::ControlManagementDialog(Experiment *experiment, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ControlManagementDialog)
{
    ui->setupUi(this);
    this->experiment=experiment;

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
