#include "approximationdialog.h"
#include "ui_approximationdialog.h"

int ApproximationDialog::lastTApproximationMethod=-1;
int ApproximationDialog::lastFApproximationMethod=-1;
QwtPlotCurve * ApproximationDialog::lastFCurve=NULL;
QwtPlotCurve * ApproximationDialog::lastTCurve=NULL;

ApproximationDialog::ApproximationDialog(Plot *plotWidget, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApproximationDialog)
{
    ui->setupUi(this);
    plot=plotWidget;

    //connect(ui->listWidget,SIGNAL(currentRowChanged(int)),SLOT(curveSelected(int)));
    tCurve=NULL;
    fCurve=NULL;
    connect(ui->TcurveListWidget,SIGNAL(currentRowChanged(int)),SLOT(TCurveSelected(int)));
    connect(ui->FcurveListWidget,SIGNAL(currentRowChanged(int)),SLOT(FCurveSelected(int)));

    /** Collect data about visible curves */
    const QwtPlotItemList& itmList = plot->itemList(QwtPlotItem::Rtti_PlotCurve);
    for ( QwtPlotItemIterator it = itmList.begin(); it != itmList.end(); ++it )
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
    ui->FcurveListWidget->addItems(curveNameList);
    ui->TcurveListWidget->addItems(curveNameList);


    if (lastTCurve!=NULL) {
        ui->TcurveListWidget->setCurrentRow(curveList.indexOf(lastTCurve));
        ui->TcurveListWidget->setEnabled(false);
    }
    if (lastFCurve!=NULL) {
        ui->FcurveListWidget->setCurrentRow(curveList.indexOf(lastFCurve));
        ui->FcurveListWidget->setEnabled(false);
    }

    switch (lastTApproximationMethod) {
    case 0:
        ui->Tapproximation0->setChecked();
        break;
    case 1:
        ui->Tapproximation1->setChecked();
        break;
    case 2:
        ui->Tapproximation2->setChecked();
        break;
    case 3:
        ui->Tapproximation3->setChecked();
        break;
    case 4:
        ui->Tapproximation4->setChecked();
        break;
    default:
        break;
    }


    switch (lastFApproximationMethod>=0) {
    case 0:
        ui->Fapproximation0->setChecked();
        break;
    case 1:
        ui->Fapproximation1->setChecked();
        break;
    case 2:
        ui->Fapproximation2->setChecked();
        break;
    case 3:
        ui->Fapproximation3->setChecked();
        break;
    case 4:
        ui->Fapproximation4->setChecked();
        break;
    default:
        break;
    }
}

ApproximationDialog::~ApproximationDialog()
{
    delete ui;
}

int ApproximationDialog::getTApproximationMethodId() const
{
    if (ui->Tapproximation0->isChecked()) return 0;
    if (ui->Tapproximation1->isChecked()) return 1;
    if (ui->Tapproximation2->isChecked()) return 2;
    if (ui->Tapproximation3->isChecked()) return 3;
    if (ui->Tapproximation4->isChecked()) return 4;

    qWarning()<<"No approximation method selected. Returning -1";
    return -1;
}

int ApproximationDialog::getFApproximationMethodId() const
{
    if (ui->Fapproximation0->isChecked()) return 0;
    if (ui->Fapproximation1->isChecked()) return 1;
    if (ui->Fapproximation2->isChecked()) return 2;
    if (ui->Fapproximation3->isChecked()) return 3;
    if (ui->Fapproximation4->isChecked()) return 4;

    qWarning()<<"No approximation method selected. Returning -1";
    return -1;
}
