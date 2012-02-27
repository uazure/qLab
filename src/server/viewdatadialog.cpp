#include "viewdatadialog.h"
#include "ui_viewdatadialog.h"

ViewDataDialog::ViewDataDialog(Experiment *experimentObject,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewDataDialog)
{
    ui->setupUi(this);
    experiment=experimentObject;
//    ui->dataListWidget->addItems(experiment->getHeader());
//    ui->dataListWidget->addItems(experiment->getData());
    ui->dataTextBrowser->setText(experiment->getHeader());
    ui->dataTextBrowser->append(experiment->getData());
    ui->transitionListWidget->addItems(experiment->getHistory());
}

ViewDataDialog::~ViewDataDialog()
{
    delete ui;
}
