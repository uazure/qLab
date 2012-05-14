#include "stringlistselectdialog.h"
#include "ui_stringlistselectdialog.h"

StringListSelectDialog::StringListSelectDialog(const QString &title, const QString &text, const QStringList &list, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StringListSelectDialog)
{

    ui->setupUi(this);
    setWindowTitle(title);
    ui->label->setText(text);
    ui->comboBox->addItems(list);
}

StringListSelectDialog::~StringListSelectDialog()
{
    delete ui;
}

void StringListSelectDialog::changeEvent(QEvent *e)
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

int StringListSelectDialog::getSelectedIndex() const {
    return ui->comboBox->currentIndex();
}
