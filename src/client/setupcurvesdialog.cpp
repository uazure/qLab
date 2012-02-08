#include "setupcurvesdialog.h"
#include "ui_setupcurvesdialog.h"

SetupCurvesDialog::SetupCurvesDialog(QWidget *parent,Plot *qwtplot,ExperimentData *experimentData) :
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

    init();

    connect(ui->curvesListWidget,SIGNAL(currentRowChanged(int)),SLOT(curveSelected(int)));
    connect(colorSelect,SIGNAL(currentIndexChanged(int)),SLOT(setCurveColor()));
    connect(ui->widthDoubleSpinBox,SIGNAL(valueChanged(double)),SLOT(setCurveWidth(double)));
    connect(ui->drawLinesCheckBox,SIGNAL(toggled(bool)),SLOT(setCurveLine(bool)));
    connect(ui->sizeSpinBox,SIGNAL(valueChanged(int)),SLOT(setCurveSymbolSize(int)));
    connect(ui->yAxisComboBox,SIGNAL(currentIndexChanged(int)),SLOT(setCurveYAxis(int)));
    connect(ui->deleteCurveButton,SIGNAL(clicked()),SLOT(deleteCurrentCurve()));
    connect(ui->addCurveButton,SIGNAL(clicked()),SLOT(addCurveDialog()));
}

SetupCurvesDialog::~SetupCurvesDialog()
{
    delete colorSelect;
    delete ui;
}

void SetupCurvesDialog::init() {
    ui->curvesListWidget->clear();
    curveList.clear();
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
    } else {
        ui->curvesGroupBox->setEnabled(true);
    }

    ui->propertiesGroupBox->setVisible(false);
    currentCurve=NULL;
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

    if (curve->yAxis()==QwtPlot::yLeft) {
        ui->yAxisComboBox->setCurrentIndex(0);
    } else {
        ui->yAxisComboBox->setCurrentIndex(1);
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

void SetupCurvesDialog::setCurveYAxis(int index) {
    if (!currentCurve) return;

    if (0==index) {
        currentCurve->setYAxis(QwtPlot::yLeft);
    } else {
        currentCurve->setYAxis(QwtPlot::yRight);
        if (!plot->axisEnabled(QwtPlot::yRight)) {
            plot->enableAxis(QwtPlot::yRight);
        }
    }
}

void SetupCurvesDialog::deleteCurrentCurve() {
    if (!currentCurve) return;
    //remove curve from curveList;
    for (int i=0;i<curveList.size();i++) {
        if (curveList.at(i)==currentCurve) {
            curveList.removeAt(i);
            break;
        }
    }
    currentCurve->detach();
    delete currentCurve;

    init();
    plot->replot();
}

void SetupCurvesDialog::addCurveDialog() {
    int maxColumn=data->columnCount(QModelIndex())-1;
    bool ok;
    int yCol = QInputDialog::getInt (this, tr("Select column to show"), tr("Select column"), 0, 0, maxColumn, 1, &ok);
    if (ok) {
        qDebug()<<"Adding curve assigned to col"<<yCol;
        plot->addCurve(yCol);
        init();
        plot->replot();
    } else {
        qWarning()<<"Would not add curve, column not selected";
    }
}

