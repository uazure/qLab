#include "approximationdialog.h"
#include "ui_approximationdialog.h"
#include "nonlinearapproximation.h"

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
    case NonLinearApproximation::methodTailAvg:
        ui->TapproximationTailAvg->setChecked(true);
        break;
    case NonLinearApproximation::methodExp:
        ui->Tapproximation1->setChecked(true);
        break;
    default:
        break;
    }


    switch (lastFApproximationMethod) {
    case NonLinearApproximation::methodLine:
        ui->Fapproximation0->setChecked(true);
        break;
    case NonLinearApproximation::methodExp:
        ui->Fapproximation1->setChecked(true);
        break;
    case NonLinearApproximation::methodExpLine:
        ui->Fapproximation2->setChecked(true);
        break;
    case NonLinearApproximation::methodExpExp:
        ui->Fapproximation3->setChecked(true);
        break;
    case NonLinearApproximation::methodExpExpLine:
        ui->Fapproximation4->setChecked(true);
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

    if (ui->Tapproximation1->isChecked()) return NonLinearApproximation::methodExp;
    if (ui->TapproximationTailAvg->isChecked()) return NonLinearApproximation::methodTailAvg;


    qWarning()<<"No approximation method selected. Returning -1";
    return -1;
}

int ApproximationDialog::getFApproximationMethodId() const
{
    if (ui->Fapproximation0->isChecked()) return NonLinearApproximation::methodLine;
    if (ui->Fapproximation1->isChecked()) return NonLinearApproximation::methodExp;
    if (ui->Fapproximation2->isChecked()) return NonLinearApproximation::methodExpLine;
    if (ui->Fapproximation3->isChecked()) return NonLinearApproximation::methodExpExp;
    if (ui->Fapproximation4->isChecked()) return NonLinearApproximation::methodExpExpLine;

    qWarning()<<"No approximation method selected. Returning -1";
    return -1;
}
