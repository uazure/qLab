#include "basicconfigurationdialog.h"
#include "ui_basicconfigurationdialog.h"
#include <QMessageBox>

BasicConfigurationDialog::BasicConfigurationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BasicConfigurationDialog)
{
    ui->setupUi(this);
    connect (ui->buttonBox,SIGNAL(clicked(QAbstractButton*)),SLOT(buttonClicked(QAbstractButton*)));
    connect (this,SIGNAL(accepted()),SLOT(saveSettings()));

    doubleValidator=new QDoubleValidator(this);

    ui->L0->setValidator(doubleValidator);

    ui->ExpA->setValidator(doubleValidator);
    ui->ExpB->setValidator(doubleValidator);
    ui->ExpC->setValidator(doubleValidator);

    ui->ExpLineA->setValidator(doubleValidator);
    ui->ExpLineB->setValidator(doubleValidator);
    ui->ExpLineC->setValidator(doubleValidator);
    ui->ExpLineD->setValidator(doubleValidator);

    ui->ExpExpLineA->setValidator(doubleValidator);
    ui->ExpExpLineB->setValidator(doubleValidator);
    ui->ExpExpLineC->setValidator(doubleValidator);
    ui->ExpExpLineD->setValidator(doubleValidator);
    ui->ExpExpLineE->setValidator(doubleValidator);
    ui->ExpExpLineF->setValidator(doubleValidator);

    settings.beginGroup("dilatometry");
    ui->L0->setText(settings.value("L0",QVariant(1)).toString());
    settings.endGroup();

    settings.beginGroup("approximation");
    ui->ExpA->setText(settings.value("Exp/a",QVariant(25000000.0)).toString());
    ui->ExpB->setText(settings.value("Exp/b",QVariant(25000000.0)).toString());
    ui->ExpC->setText(settings.value("Exp/c",QVariant(60.0)).toString());

    ui->ExpLineA->setText(settings.value("ExpLine/a",QVariant(25000000.0)).toString());
    ui->ExpLineB->setText(settings.value("ExpLine/b",QVariant(25000000.0)).toString());
    ui->ExpLineC->setText(settings.value("ExpLine/c",QVariant(60.0)).toString());
    ui->ExpLineD->setText(settings.value("ExpLine/d",QVariant(0.5)).toString());

    ui->ExpExpLineA->setText(settings.value("ExpExpLine/a",QVariant(25000000.0)).toString());
    ui->ExpExpLineB->setText(settings.value("ExpExpLine/b",QVariant(25000000.0)).toString());
    ui->ExpExpLineC->setText(settings.value("ExpExpLine/c",QVariant(60.0)).toString());
    ui->ExpExpLineD->setText(settings.value("ExpExpLine/d",QVariant(60.0)).toString());
    ui->ExpExpLineE->setText(settings.value("ExpExpLine/e",QVariant(25000000.0)).toString());
    ui->ExpExpLineF->setText(settings.value("ExpExpLine/f",QVariant(0.5)).toString());

    settings.endGroup();

}

BasicConfigurationDialog::~BasicConfigurationDialog()
{
    delete ui;
    delete doubleValidator;
}

void BasicConfigurationDialog::changeEvent(QEvent *e)
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


void BasicConfigurationDialog::buttonClicked(QAbstractButton *but) {
    if(ui->buttonBox->standardButton(but) == QDialogButtonBox::Apply) {
        saveSettings();
        }
}

void BasicConfigurationDialog::saveSettings() {

    settings.beginGroup("dilatometry");
    settings.setValue("L0",ui->L0->text().toDouble());
    settings.endGroup();

    settings.beginGroup("approximation");
    settings.setValue("Exp/a",ui->ExpA->text().toDouble());
    settings.setValue("Exp/b",ui->ExpB->text().toDouble());
    settings.setValue("Exp/c",ui->ExpC->text().toDouble());

    settings.setValue("ExpLine/a",ui->ExpLineA->text().toDouble());
    settings.setValue("ExpLine/b",ui->ExpLineB->text().toDouble());
    settings.setValue("ExpLine/c",ui->ExpLineC->text().toDouble());
    settings.setValue("ExpLine/d",ui->ExpLineD->text().toDouble());

    settings.setValue("ExpExpLine/a",ui->ExpExpLineA->text().toDouble());
    settings.setValue("ExpExpLine/b",ui->ExpExpLineB->text().toDouble());
    settings.setValue("ExpExpLine/c",ui->ExpExpLineC->text().toDouble());
    settings.setValue("ExpExpLine/d",ui->ExpExpLineD->text().toDouble());
    settings.setValue("ExpExpLine/e",ui->ExpExpLineE->text().toDouble());
    settings.setValue("ExpExpLine/f",ui->ExpExpLineF->text().toDouble());
    settings.endGroup();

    settings.sync();
    if (settings.status()!=QSettings::NoError) {
        QMessageBox::warning(NULL,"Failed to save settings","Can not save settings");
        return;
    }
}
