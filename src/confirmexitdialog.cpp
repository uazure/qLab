#include "confirmexitdialog.h"
#include "ui_confirmexitdialog.h"

ConfirmExitDialog::ConfirmExitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmExitDialog)
{
    ui->setupUi(this);
}

ConfirmExitDialog::~ConfirmExitDialog()
{
    delete ui;
}

void ConfirmExitDialog::changeEvent(QEvent *e)
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

void ConfirmExitDialog::setQuestion(QString question) {
    ui->question->setText(question);
}
