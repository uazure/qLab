#include "plot.h"


Plot::Plot(QWidget *parent, ExperimentData *data) :
        QwtPlot(parent)
{
    xCol=-1;
    experimentData=data;
    dataTable=data->getDataTable();
}

Plot::~Plot() {
//this->detachItems();
}

void Plot::addCurve(int yColumn, int xColumn) {
    if (xColumn<0) xColumn=0;
    if (yColumn>dataTable->size() || yColumn<0 || xColumn>dataTable->size()) {
        qWarning()<<"yColumn is"<<yColumn<<"and xColumn is"<<xColumn<<"and number of columns is"<<dataTable->size();
        return;
    }
    QwtPlotCurve *plotCurve=new QwtPlotCurve(experimentData->getColumnLabel(yColumn));
    SeriesData *series=new SeriesData(experimentData->getDataTable(), yColumn, xColumn);
    plotCurve->setData(series);
    QwtSymbol *symbol = new QwtSymbol (
            QwtSymbol::Triangle,        //default is the triangle
            QBrush(Qt::black),          //default color
            QPen(Qt::NoPen),            //empty pen
            QSize(7,7));                //hard-coded size
    plotCurve->setSymbol(symbol);
    plotCurve->attach(this);
}

void Plot::clear() {
    xCol=-1;
    detachItems( QwtPlotItem::Rtti_PlotItem, autoDelete() );
}

void Plot::initialize() {
    clear();
    if (dataTable->size()<1) {
        qWarning()<<"dataTable is empty! Can not initialize";
        return;
    }
    for (int i=0;i<dataTable->size();i++) {
        if (experimentData->getColumnAxis(i)==xBottom) {
            qDebug()<<"Found xBottom at column"<<i;
            xCol=i;
        }
    }

    if (xCol<0) {
        qWarning()<<"Failed to find xBottom axis from data source";
        return;
    }

    for (int i=0;i<dataTable->size();i++) {
        if (experimentData->getColumnAxis(i)!=xBottom && experimentData->getColumnAxis(i)!=xTop) {
            qDebug()<<"Adding curve for column"<<i;
            addCurve(i,xCol);
        }
    }
}
