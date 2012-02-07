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

    colorSelect=new ColorBox(this);
    ui->colorSelectLayout->addWidget(colorSelect);

    //populate curevesListWidget with current curves on the plot
    const QwtPlotItemList& itmList = plot->itemList(QwtPlotItem::Rtti_PlotCurve);
    //iterate over all plot items of type PlotCurve
    for (QwtPlotItemIterator it = itmList.begin(); it != itmList.end(); ++it ) {
        QwtPlotCurve *curve=(QwtPlotCurve*)(*it);
        ui->curvesListWidget->addItem(curve->title().text());
        curveList.append(curve);
    }

    if (curveList.isEmpty()) {
        ui->curvesGroupBox->setEnabled(false);
    }

    ui->propertiesGroupBox->setVisible(false);
    currentCurve=NULL;

    connect(ui->curvesListWidget,SIGNAL(currentRowChanged(int)),SLOT(curveSelected(int)));
    connect(colorSelect,SIGNAL(currentIndexChanged(int)),SLOT(setCurveColor()));
    connect(ui->widthDoubleSpinBox,SIGNAL(valueChanged(double)),SLOT(setCurveWidth(double)));
    connect(ui->drawLinesCheckBox,SIGNAL(toggled(bool)),SLOT(setCurveLine(bool)));
    connect(ui->sizeSpinBox,SIGNAL(valueChanged(int)),SLOT(setCurveSymbolSize(int)));
}

SetupCurvesDialog::~SetupCurvesDialog()
{
    delete colorSelect;
    delete ui;
}

void SetupCurvesDialog::curveSelected(int index)
{
    if (index>=curveList.size() || index <0) {
        ui->propertiesGroupBox->setVisible(false);
        currentCurve=NULL;
        return;
    }

    ui->propertiesGroupBox->setVisible(true);

    //get curve pointer
    QwtPlotCurve *curve=curveList[index];
    currentCurve=curve;
    //get symbol color of currently selected curve
    QColor color=curve->symbol()->brush().color();
    //set this color to colorSelect box
    colorSelect->setColor(color);
    //set size (width) of symbol to spinbox
    ui->sizeSpinBox->setValue(curve->symbol()->size().width());

    ui->widthDoubleSpinBox->setValue(curve->pen().widthF());

    //set drawLinesCheckBox according to curve settings
    if (curve->style()==QwtPlotCurve::NoCurve) {
        ui->drawLinesCheckBox->setChecked(false);
    } else {
        ui->drawLinesCheckBox->setChecked(true);
    }



}

void SetupCurvesDialog::setCurveColor()
{
    if (!currentCurve) return;
    QColor color=colorSelect->color();

    QwtSymbol *symbol=const_cast<QwtSymbol *> (currentCurve->symbol());
    symbol->setColor(color);
    currentCurve->setSymbol(symbol);
    QPen pen=const_cast<QPen&>(currentCurve->pen());
    pen.setColor(color);
    currentCurve->setPen(pen);
}

void SetupCurvesDialog::setCurveWidth(double width)
{
    if (!currentCurve) return;

    QPen pen=const_cast<QPen&>(currentCurve->pen());
    pen.setWidthF((qreal)width);
    currentCurve->setPen(pen);
}

void SetupCurvesDialog::setCurveLine(bool on)
{
    if (!currentCurve) return;

    if (on) {
        currentCurve->setStyle(QwtPlotCurve::Lines);
    } else {
        currentCurve->setStyle(QwtPlotCurve::NoCurve);
    }
}

void SetupCurvesDialog::setCurveSymbolSize(int size)
{
    if (!currentCurve) return;

    QwtSymbol *symbol=const_cast<QwtSymbol *> (currentCurve->symbol());
    symbol->setSize(size);
    currentCurve->setSymbol(symbol);
}
