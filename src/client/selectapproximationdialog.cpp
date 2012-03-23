#include "selectapproximationdialog.h"
#include "ui_selectapproximationdialog.h"

SelectApproximationDialog::SelectApproximationDialog(const QString &name, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectApproximationDialog)
{
    ui->setupUi(this);
    QFont font=QWidget::font();
    font.setPointSizeF(font.pointSizeF()*1.3);
    font.setBold(true);
    ui->label->setFont(font);
    ui->label->setText(name);
}

SelectApproximationDialog::~SelectApproximationDialog()
{
    delete ui;
}

void SelectApproximationDialog::changeEvent(QEvent *e)
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

int SelectApproximationDialog::getApproximationMethodId() const {
    if (ui->approximation0->isChecked()) return 0;
    if (ui->approximation1->isChecked()) return 1;
    if (ui->approximation2->isChecked()) return 2;
    if (ui->approximation3->isChecked()) return 3;
    if (ui->approximation4->isChecked()) return 4;

    qWarning()<<"No approximation method selected. Returning -1";
    return -1;
}
