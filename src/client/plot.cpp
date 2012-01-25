#include "plot.h"


Plot::Plot(QWidget *parent, ExperimentData *data) :
        QwtPlot(parent)
{
    xCol=-1;
    experimentData=data;
    dataTable=data->getDataTable();
    setAutoDelete(true);
    setAutoReplot(false);

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

    // if curve should be plotted in xBottom-yRight axes
    if (experimentData->getColumnAxis(yColumn)==yRight) {
        plotCurve->setYAxis(yRight);
    }

    // specify a point marker (so called 'symbol') for curve
    QwtSymbol *symbol = new QwtSymbol (
            QwtSymbol::Triangle,        //default is the triangle
            QBrush(Qt::black),          //default color
            QPen(Qt::NoPen),            //empty pen
            QSize(7,7));                //hard-coded size
    plotCurve->setSymbol(symbol);
    plotCurve->attach(this);
}

void Plot::clear() {
    // set xCol to -1 to indicate that absciss axis is not defined in data object to prevent automatic curve generation
    xCol=-1;
    // detach all items from the plot
    detachItems( QwtPlotItem::Rtti_PlotItem, autoDelete() );
    //disable yRight and xTop axes if they were enabled for some reason
    enableAxis(yRight,false);
    enableAxis(xTop,false);
    replot();
}

void Plot::initialize() {
    // Clear plot from all items
    clear();
    // re-create plot grids
    QwtPlotGrid *grid=new QwtPlotGrid;
    grid->setMajPen(QPen(Qt::darkBlue,0,Qt::SolidLine));
    grid->attach(this);

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

    bool rightGridEnabled=false;
    for (int i=0;i<dataTable->size();i++) {
        if (experimentData->getColumnAxis(i)!=xBottom && experimentData->getColumnAxis(i)!=xTop &&
                (experimentData->getColumnAxis(i)==yLeft || experimentData->getColumnAxis(i)==yRight) ) {
            qDebug()<<"Adding curve for column"<<i;

            // create grid for yRight if we have at least one curve at yRight axis
            if (experimentData->getColumnAxis(i)==yRight && !rightGridEnabled) {
                QwtPlotGrid *grid=new QwtPlotGrid;
                grid->setMajPen(QPen(Qt::darkRed,0,Qt::SolidLine));
                // disable grid for X axis, because vertical grid will be painted using grid attached to xLeft axis
                grid->enableX(false);
                grid->setYAxis(yRight);
                grid->attach(this);
            }

            // enable yRight axis
            enableAxis(experimentData->getColumnAxis(i));

            // add curve to the plot
            addCurve(i,xCol);
        }
    }
}
