#include "selectcurvedialog.h"
#include "ui_selectcurvedialog.h"

#include "plot.h"


//itemName - name of what we want to select. I.e. Temperature, Pressure
SelectCurveDialog::SelectCurveDialog(const QString &itemName, Plot *plot) :
    QDialog(plot),
    ui(new Ui::selectCurveDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Select")+" "+itemName);
    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),SLOT(curveSelected(int)));
    selectedCurve=NULL;

    /** Collect data about visible curves */
    const QwtPlotItemList& itmList = plot->itemList(QwtPlotItem::Rtti_PlotCurve);
    for ( QwtPlotItemIterator it = itmList.begin();
         it != itmList.end(); ++it )
    {
        QwtPlotCurve *c = (QwtPlotCurve*)(*it);
        // if plot item is visible and is in legend then create curve for selection markers
        if (c->isVisible() && c->testItemAttribute(QwtPlotItem::Legend))
        {
            curveList.append(c);
            curveNameList.append(c->title().text());
        }
    }

    /** Populate selection window with all available curves*/
    ui->listWidget->addItems(curveNameList);

}
SelectCurveDialog::~SelectCurveDialog()
{
    delete ui;
}

void SelectCurveDialog::changeEvent(QEvent *e)
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


void SelectCurveDialog::curveSelected(int index) {
    selectedCurve=curveList.at(index);
}

QwtPlotCurve * SelectCurveDialog::getSelectedCurve() const {
    return selectedCurve;
}

bool SelectCurveDialog::isRememberChecked() const {
    return ui->checkBox->isChecked();
}
