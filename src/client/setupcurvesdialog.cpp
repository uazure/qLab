#include "setupcurvesdialog.h"
#include "ui_setupcurvesdialog.h"

SetupCurvesDialog::SetupCurvesDialog(QWidget *parent,QwtPlot *qwtplot,ExperimentData *experimentData) :
    QDialog(parent),
    ui(new Ui::SetupCurvesDialog)
{
    ui->setupUi(this);
    plot=qwtplot;
    data=experimentData;
    ui->asciiList->addItems(data->getAscii());
    ui->tableView->setModel(data);

    //populate curevesListWidget with current curves on the plot
    const QwtPlotItemList& itmList = plot->itemList(QwtPlotItem::Rtti_PlotCurve);
    //iterate over all plot items of type PlotCurve
    for (QwtPlotItemIterator it = itmList.begin(); it != itmList.end(); ++it ) {
        QwtPlotCurve *curve=(QwtPlotCurve*)(*it);
        ui->curvesListWidget->addItem(curve->title().text());

    }

}

SetupCurvesDialog::~SetupCurvesDialog()
{
    delete ui;
}
