#include "plot.h"
//#include "abstractinterpolation.h"
#include "selectcurvedialog.h"

int Plot::markerCount=0;

Plot::Plot(QWidget *parent, ExperimentData *data) :
        QwtPlot(parent)
{
    //should be faster on platforms that support this (unix)
#ifdef Q_WS_X11
    qDebug()<<"Enabling Qt::WA_PaintOutsidePaintEvent";
    canvas()->setAttribute(Qt::WA_PaintOutsidePaintEvent, true);
#endif
    incrementalDraw=false;
    showInterpolation=true;
    //set default monitoring time to 1 minute (60 seconds)
    setMonitoringInterval(60);

    xCol=-1;
    T0index=-1;
    selectedCurve=NULL; // set null pointer (i.e. no curve selected)
    selectedPoint=-1; //no point selected. 0 - means first point of the curve.
    experimentData=data;
    dataTable=data->getDataTable();
    setAutoDelete(true);
    setAutoReplot(false);

    //legend
    QwtLegend *legend = new QwtLegend(this);
    //legend->setFrameStyle(QFrame::Box|QFrame::Sunken);
    legend->setItemMode(QwtLegend::CheckableItem);
    insertLegend(legend, QwtPlot::BottomLegend);

    //click on legend item allows to hide items from the plot
    connect(this,SIGNAL(legendChecked(QwtPlotItem*,bool)),this, SLOT(hidePlotItem(QwtPlotItem*,bool)));

//Picker with click point machine to provide point selection
    selectPointPicker=new QwtPicker(canvas());
    selectPointPicker->setStateMachine(new QwtPickerClickPointMachine);
    connect(selectPointPicker,SIGNAL(appended(QPoint)),SLOT(selectPoint(QPoint)));

//Picker for selecting range of points (Shift+LMB)
    selectRangePicker=new QwtPicker(canvas());
    selectRangePicker->setStateMachine(new QwtPickerClickPointMachine);
    selectRangePicker->setMousePattern(QwtPicker::MouseSelect1,Qt::LeftButton,Qt::SHIFT);
    connect(selectRangePicker,SIGNAL(appended(QPoint)),SLOT(selectRange(QPoint)));

//Picker for append/remove additional points to/from selection (Ctrl+LMB)
    appendPointPicker=new QwtPicker(canvas());
    appendPointPicker->setStateMachine(new QwtPickerClickPointMachine);
    appendPointPicker->setMousePattern(QwtPicker::MouseSelect1,Qt::LeftButton,Qt::CTRL);
    connect(appendPointPicker,SIGNAL(appended(QPoint)),SLOT(appendPoint(QPoint)));

/** \brief zoomers are used to zoom by selecting rectangle on canvas
    In general there's 4 zoomers on the canvas
    1 and 2 - zoomerLeft and zoomerRight. They are activated by LMB w/o modificators.
    They work simultaneously to allow adequate zooming of yLeft and yRight axes
    3 - zoomerExclusiveLeft. It's activated with Ctrl+LMB and works for yLeft axis only
    4 - zoomerExclusiveRight. It's activated with Shift+LMB and works for yRight axis only

    All zoomers are disabled when user switches to point select mode and enabled when
    it switches back to normal mode.
    \sa selectPointsMode();

    Normally, zoomer uses RMB to zoom out to original state and MMB to zoom out 1 step.
    We don't want zoom out to original with RMB because it's assigned to panner.
    But zoom out 1 step can be performed with MMB for yLeft and yRight simultaneously,
    or by Ctrl+MMB for yLeft axis only and Shift+MMB for yRight axis only

    FIXME: zoomers should set it's base zoom to current canvas view after panning and magnifying
    Probable solution: implement signals to QwtPlotPanner and QwtPlotMagnifier and slot for QwtPlotZoomer
  */

    zoomerLeft=new QwtPlotZoomer(xBottom,yLeft,canvas());
    //disable zoom to base with RMB (RMB is used by panner)
    zoomerLeft->setMousePattern(QwtPicker::MouseSelect2,Qt::NoButton);
    //disable zoom one step back with [MouseSelect3] (MMB)
    zoomerLeft->setMousePattern(QwtPicker::MouseSelect3,Qt::NoButton);
    //disable tracker
    zoomerLeft->setTrackerMode(QwtPicker::AlwaysOff);

    zoomerRight=new QwtPlotZoomer(xTop,yRight,canvas());
    //disable zoom to base with RMB [MouseSelect2] (RMB is used by panner)
    zoomerRight->setMousePattern(QwtPicker::MouseSelect2,Qt::NoButton);
    //disable zoom one step back with [MouseSelect3] (MMB)
    zoomerRight->setMousePattern(QwtPicker::MouseSelect3,Qt::NoButton);
    //disable selection rectangle for yRight
    zoomerRight->setRubberBand(QwtPicker::NoRubberBand);
    zoomerRight->setTrackerMode(QwtPicker::AlwaysOff);

    //initialization of exclusive yLeft and yRight zoomers
    zoomerExclusiveLeft= new QwtPlotZoomer(xBottom,yLeft,canvas());
    zoomerExclusiveLeft->setMousePattern(QwtPicker::MouseSelect1,Qt::LeftButton,Qt::CTRL);
    zoomerExclusiveLeft->setMousePattern(QwtPicker::MouseSelect2,Qt::NoButton);
    zoomerExclusiveLeft->setMousePattern(QwtPicker::MouseSelect3,Qt::MidButton,Qt::CTRL);
    zoomerExclusiveLeft->setTrackerMode(QwtPicker::AlwaysOff);

    zoomerExclusiveRight= new QwtPlotZoomer(xBottom,yRight,canvas());
    zoomerExclusiveRight->setMousePattern(QwtPicker::MouseSelect1,Qt::LeftButton,Qt::SHIFT);
    zoomerExclusiveRight->setMousePattern(QwtPicker::MouseSelect2,Qt::NoButton);
    zoomerExclusiveRight->setMousePattern(QwtPicker::MouseSelect3,Qt::MidButton,Qt::SHIFT);
    zoomerExclusiveRight->setTrackerMode(QwtPicker::AlwaysOff);


    //Panner is working with right mouse button
    QwtPlotPanner *panner=new QwtPlotPanner(canvas());
    panner->setMouseButton(Qt::RightButton);
    panner->setAxisEnabled(yRight,true);



    //Magnifier is set to zoom in/out on mouse wheel only
    magnifier=new QwtPlotMagnifier(canvas());
    //Disable magnifying with RMB+MouseMove up/down
    magnifier->setMouseButton(Qt::NoButton);
    magnifier->setAxisEnabled(yRight,true);
    magnifier->setWheelFactor(1.414);

    /*Special magnifiers that can be used to scale one axis only
      and that are activated with mouse over selected axis widget
      */

    yLeftMagnifier=new ScalePlotMagnifier(canvas());
    yLeftMagnifier->setAxisEnabled(xBottom,false);
    yLeftMagnifier->setAxisEnabled(xTop,false);
    yLeftMagnifier->setAxisEnabled(yRight,false);
    axisWidget(yLeft)->installEventFilter(yLeftMagnifier);
    canvas()->removeEventFilter(yLeftMagnifier);

    yRightMagnifier=new ScalePlotMagnifier(canvas());
    yRightMagnifier->setAxisEnabled(xBottom,false);
    yRightMagnifier->setAxisEnabled(xTop,false);
    yRightMagnifier->setAxisEnabled(yLeft,false);
    axisWidget(yRight)->installEventFilter(yRightMagnifier);
    canvas()->removeEventFilter(yRightMagnifier);

    xBottomMagnifier=new ScalePlotMagnifier(canvas());
    xBottomMagnifier->setAxisEnabled(yRight,false);
    xBottomMagnifier->setAxisEnabled(xTop,false);
    xBottomMagnifier->setAxisEnabled(yLeft,false);
    axisWidget(xBottom)->installEventFilter(xBottomMagnifier);
    canvas()->removeEventFilter(xBottomMagnifier);

    xBottomPanner=new ScalePlotPanner(canvas());
    xBottomPanner->setAxisEnabled(QwtPlot::yLeft,false);
    xBottomPanner->setAxisEnabled(QwtPlot::yRight,false);
    xBottomPanner->setAxisEnabled(QwtPlot::xTop,false);
    xBottomPanner->setMouseButton(Qt::RightButton);
    axisWidget(xBottom)->installEventFilter(xBottomPanner);
    canvas()->removeEventFilter(xBottomPanner);

    yLeftPanner=new ScalePlotPanner(canvas());
    yLeftPanner->setAxisEnabled(QwtPlot::xBottom,false);
    yLeftPanner->setAxisEnabled(QwtPlot::yRight,false);
    yLeftPanner->setAxisEnabled(QwtPlot::xTop,false);
    yLeftPanner->setMouseButton(Qt::RightButton);
    axisWidget(yLeft)->installEventFilter(yLeftPanner);
    canvas()->removeEventFilter(yLeftPanner);

    yRightPanner=new ScalePlotPanner(canvas());
    yRightPanner->setAxisEnabled(QwtPlot::xBottom,false);
    yRightPanner->setAxisEnabled(QwtPlot::yLeft,false);
    yRightPanner->setAxisEnabled(QwtPlot::xTop,false);
    yRightPanner->setMouseButton(Qt::RightButton);
    axisWidget(yRight)->installEventFilter(yRightPanner);
    canvas()->removeEventFilter(yRightPanner);


    //it's safe to call initialize even without data. It will reset plot to default state, add grids, etc.
    initialize();
}

Plot::~Plot() {
    clear();
}

void Plot::addCurve(int yColumn, int xColumn) {
    if (xColumn<0) xColumn=0;
    if (yColumn>dataTable->size() || yColumn<0 || xColumn>dataTable->size()) {
        qWarning()<<"yColumn is"<<yColumn<<"and xColumn is"<<xColumn<<"and number of columns is"<<dataTable->size();
        return;
    }
    QwtPlotCurve *plotCurve=new QwtPlotCurve(experimentData->getColumnLabel(yColumn));
    plotCurve->setLegendAttribute(QwtPlotCurve::LegendShowLine);
    plotCurve->setLegendAttribute(QwtPlotCurve::LegendShowSymbol);
    SeriesData *series=new SeriesData(experimentData->getDataTable(), yColumn, xColumn);
    plotCurve->setData(series);

    // if curve should be plotted in xBottom-yRight axes
    if (experimentData->getColumnAxis(yColumn)==yRight) {
        plotCurve->setYAxis(yRight);
    }

    // specify a point marker (so called 'symbol') for curve
    QwtSymbol *symbol = new QwtSymbol (
            QwtSymbol::Triangle,        //default is the triangle
            QBrush(Qt::blue),          //default color
            QPen(Qt::NoPen),            //empty pen
            QSize(7,7));                //hard-coded size

    if (experimentData->getColumnAxis(yColumn)==yRight) {
        symbol->setBrush(QBrush(Qt::red));
        plotCurve->setPen(QPen(Qt::red));
    } else {
        plotCurve->setPen(QPen(Qt::blue));
    }

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
    setAxisTitle(yLeft,"");
    setAxisTitle(yRight,"");
    //setAxisTitle(xBottom,"");
    setAxisTitle(xTop,"");
    replot();
}

void Plot::initialize() {
    //set normal selection mode
    selectPointsMode(false);


    // Clear plot from all items
    clear();
    // re-create plot grids
    QwtPlotGrid *grid=new QwtPlotGrid;
    grid->setMajPen(QPen(Qt::darkBlue,0,Qt::SolidLine));
    grid->attach(this);


    if (dataTable->size()<1) {
        qWarning()<<"dataTable is empty! Can not initialize";
        replot();
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
    QwtPlot::Axis axis;
    for (int i=0;i<dataTable->size();i++) {
        axis=experimentData->getColumnAxis(i);
        if (axis==yLeft || axis==yRight) {
            qDebug()<<"Adding curve for column"<<i;

            // create grid for yRight if we have at least one curve at yRight axis
            if (axis==yRight && !rightGridEnabled) {
                QwtPlotGrid *grid=new QwtPlotGrid;
                grid->setMajPen(QPen(Qt::darkRed,0,Qt::SolidLine));
                // disable grid for X axis, because vertical grid will be painted using grid attached to xLeft axis
                grid->enableX(false);
                grid->setYAxis(yRight);
                grid->attach(this);
            }

            // enable axis
            enableAxis(axis);

            // add curve to the plot
            addCurve(i,xCol);

            //set axis title if it's not set
            if (axisTitle(axis).text().isEmpty()) {
                setAxisTitle(axis,experimentData->getColumnLabel(i)+", "+experimentData->getColumnUnit(i));
            }
        }
    }

}

void Plot::hidePlotItem(QwtPlotItem *plotItem, bool hide)
{
    qDebug()<<"Hiding item"<<hide;
    plotItem->setVisible(!hide);
    replot();
}


void Plot::markSelectedPoints()
{
    /** dataList is temporary map of vectors.
    Each vector contains data for its QwtPlotCurve (key())
      */
    dataMap.clear();

    if (selectedCurve==NULL || selectedPoints.isEmpty()) {
        selectedCurve=NULL;
        selectedPoints.clear();

        if (!markCurveMap.isEmpty()) {
            for (QMap<QwtPlotCurve*,QwtPlotCurve*>::iterator iterator = markCurveMap.begin();iterator!=markCurveMap.end();++iterator) {
                iterator.value()->detach();
                delete iterator.value();
            }
            markCurveMap.clear();
        }
         return;
    }

    //create markCurveList
    if (markCurveMap.isEmpty()) {
        //iterate over all QwtPlotCurves that are on plot
        const QwtPlotItemList& itmList = itemList(QwtPlotItem::Rtti_PlotCurve);
        for ( QwtPlotItemIterator it = itmList.begin();
             it != itmList.end(); ++it )
        {
            QwtPlotCurve *c = (QwtPlotCurve*)(*it);
            // if plot item is visible and is in legend then create curve for selection markers
            if (c->isVisible() && c->testItemAttribute(QwtPlotItem::Legend))
            {
                QwtPlotCurve *newCurve=new QwtPlotCurve();
                newCurve->setItemAttribute(QwtPlotItem::Legend,false);
                newCurve->setStyle(QwtPlotCurve::NoCurve);
                QwtSymbol *symbol=new QwtSymbol(*c->symbol());
                symbol->setSize((symbol->size())*1.5);
                //newCurve->setSymbol(new QwtSymbol(QwtSymbol::Ellipse,QBrush(c->symbol()->brush()),QPen(Qt::NoPen),QSize(9,9)));
                newCurve->setSymbol(symbol);
                newCurve->setZ(50);
                newCurve->setAxes(c->xAxis(),c->yAxis());
                newCurve->attach(this);
                markCurveMap.insert(c,newCurve);
                //create vector associated with the created curve
                dataMap.insert(newCurve,QVector<QPointF>());
            }
        }
    }

    //iterate over indexes of selected points and add data to vectors
    for (QMap<int,QPointF>::const_iterator i=selectedPoints.constBegin();i!=selectedPoints.constEnd();++i) {
        /** for each selected point index we would iterate over all visible curves
          */
        const QwtPlotItemList& itmList = itemList(QwtPlotItem::Rtti_PlotCurve);
        for (QwtPlotItemIterator it = itmList.begin(); it != itmList.end(); ++it) {
            //*c is regular visible curve on the plot
            QwtPlotCurve *c = (QwtPlotCurve*)(*it);
            //if *c is also present in legend (not the curve with markers)
            if (c->isVisible() && c->testItemAttribute(QwtPlotItem::Legend)) {
                //*nc is curve with marked points for *c
                QwtPlotCurve *nc = markCurveMap.value(c);
                if (nc==NULL) {
                    qWarning()<<"Empty marker curve associated with curve"<<c->title().text();
                    continue;
                }
                //dataMap[nc] - vector with data for *nc
                //We would append there a point from original curve index sample(i.key)
                dataMap[nc].append(c->sample(i.key()));
            }
        }
    }

    if (!markCurveMap.isEmpty()) {
        for (QMap<QwtPlotCurve*,QwtPlotCurve*>::iterator iterator = markCurveMap.begin();iterator!=markCurveMap.end();++iterator) {
            iterator.value()->setSamples(dataMap.value(iterator.value()));
        }
    }
}



void Plot::replot()
{
    markSelectedPoints();
#if QT_VERSION >= 0x040700
    QElapsedTimer timer;
    timer.start();
#endif
    canvas()->setPaintAttribute( QwtPlotCanvas::ImmediatePaint, true);
    QwtPlot::replot();
    canvas()->setPaintAttribute( QwtPlotCanvas::ImmediatePaint, false);

#if QT_VERSION >= 0x040700
    qDebug()<<"Replot took"<<timer.restart()<<"msecs";
#endif

}

void Plot::selectPoint(const QPoint &point) {

    selectedCurve=NULL;
    selectedPoint=-1;
    selectedPoints.clear();

    if (getCurvePoint(point)) {
        selectedPoints.insert(selectedPoint,selectedCurve->sample(selectedPoint));
    } else {
        selectedCurve=NULL;
        selectedPoint=-1;
        selectedPoints.clear();
    }
    replot();

}

void Plot::selectRange(const QPoint &point)
{
    if (selectedCurve==NULL) {
        selectPoint(point);
        return;
    }

    int i=selectedPoint;
    if (getCurvePoint(point,selectedCurve)) {
        if (i<selectedPoint) {
            for (int j=i;j<=selectedPoint;j++) {
                selectedPoints.insert(j,selectedCurve->sample(j));
            }
        } else if (i>selectedPoint) {
            for (int j=selectedPoint;j<=i;j++) {
                selectedPoints.insert(j,selectedCurve->sample(j));
            }
        } // else if (i==selectedPoint) do nothing :)
    }
    replot();
}

void Plot::appendPoint(const QPoint &point)
{
    if (selectedCurve==NULL) {
        selectPoint(point);
        return;
    }

    if (getCurvePoint(point,selectedCurve)) {
        if (selectedPoints.contains(selectedPoint)) {
            qDebug()<<"Removing point from selection";
            selectedPoints.remove(selectedPoint);
        } else {
            qDebug()<<"Appending point to selection";
            selectedPoints.insert(selectedPoint,selectedCurve->sample(selectedPoint));
        }
    replot();
    } else {
        qDebug()<<"Failed to get curve point near"<<point;
    }
}

bool Plot::getCurvePoint(const QPoint &point, QwtPlotCurve *curve)
{
    qDebug()<<"Point selected:"<<point;
    double dist = 10e10;
    int index = -1;
    if (curve!=NULL) {
        index=curve->closestPoint(point,&dist);
    } else {
        const QwtPlotItemList& itmList = itemList(QwtPlotItem::Rtti_PlotCurve);
        //iterate over all plot items
        for ( QwtPlotItemIterator it = itmList.begin();
             it != itmList.end(); ++it )
        {
            // if plot item is of type PlotCurve then find closest point
            if ((QwtPlotCurve*)(*it)->isVisible())
            {
                QwtPlotCurve *c = (QwtPlotCurve*)(*it);
                double d;
                int idx = c->closestPoint(point, &d);
                if ( d < dist )
                {
                    curve = c;
                    index = idx;
                    dist = d;
                }
            }
        }
    }
    // if there's a curve and index and it's closer than 15px then emit signal
    if (curve!=NULL && index !=-1 && dist < 15) {
        qDebug()<<"Curve point"<< index<<"selected on curve"<<curve->title().text();
        selectedCurve=curve;
        selectedPoint=index;
        emit curvePointClicked(curve,index);
        emit xValueSelected(curve->sample(index).x());
        return true;
    } else {
        return false;
    }
}

void Plot::normalMode() {
    selectPointsMode(false);
}

void Plot::selectPointsMode(bool select) {
    selectPointPicker->setEnabled(select);
    selectRangePicker->setEnabled(select);
    appendPointPicker->setEnabled(select);
    zoomerLeft->setEnabled(!select);
    zoomerRight->setEnabled(!select);
    zoomerExclusiveLeft->setEnabled(!select);
    zoomerExclusiveRight->setEnabled(!select);
    if (select) emit message("Select mode");
    else emit message("Normal mode");
}

void Plot::drawLastPoint(int size) {
    if (!incrementalDraw) return;
    int lastPoint=--size;
    double maxXValue=0.0;
    double tmp;
    const QwtPlotItemList& itmList = itemList(QwtPlotItem::Rtti_PlotCurve);
    //iterate over all plot items of type PlotCurve
    for (QwtPlotItemIterator it = itmList.begin(); it != itmList.end(); ++it ) {
        QwtPlotCurve *curve=(QwtPlotCurve*)(*it);
        tmp=curve->boundingRect().right();
        if (tmp>maxXValue) {
            maxXValue=tmp;
        }
    }
    qDebug()<<"Max X values is"<<tmp;
    setAxisScale(xBottom,maxXValue-monitoringInterval,maxXValue);
    QwtPlot::replot();
}

void Plot::zoomExtents(void)
{
    setAxisAutoScale(xBottom,true);
    setAxisAutoScale(yLeft,true);
    setAxisAutoScale(yRight,true);
    replot();
    setAxisAutoScale(xBottom,false);
    setAxisAutoScale(yLeft,false);
    setAxisAutoScale(yRight,false);
}

void Plot::setIncrementalDraw(bool on)
{
    if (on) replot();
    incrementalDraw=on;
}

void Plot::setMonitoringInterval(double interval)
{
    if (interval > 30)  monitoringInterval=interval;
    else {
        qDebug()<<"Failed to set monitoring interval less than 30 seconds:"<<interval;
        emit message("Failed to set interval less than 30 seconds");
    }
}

double Plot::getMonitoringInterval(void) const
{
    return monitoringInterval;
}


void Plot::appendMarker(QPointF point, int control) {
    QwtPlotMarker *marker=new QwtPlotMarker();
    marker->setLineStyle(QwtPlotMarker::VLine);
    QColor color(Qt::red);
    QBrush redbrush(Qt::red);
    QPen linePen(redbrush,2,Qt::SolidLine);
    marker->setLinePen(linePen);
    marker->setValue(point);
    marker->setTitle(QString::number(control));
    marker->attach(this);
    emit message("Appended marker");
}

void Plot::appendMarker(int rowIndex) {
    const QwtPlotItemList& itmList = itemList(QwtPlotItem::Rtti_PlotCurve);
    //iterate over all plot items of type PlotCurve
    for (QwtPlotItemIterator it = itmList.begin(); it != itmList.end(); ++it ) {
        QwtPlotCurve *curve=(QwtPlotCurve*)(*it);
        if (!curve->testItemAttribute(QwtPlotItem::Legend)) {
            continue;
        }
        QwtPlotMarker *marker=new QwtPlotMarker();
        //marker->setLineStyle(QwtPlotMarker::NoLine);
        QColor color(Qt::red);
        QBrush redbrush(Qt::red);
        QwtSymbol *symbol=new QwtSymbol(*curve->symbol());
        QPen pen(Qt::SolidLine);
        pen.setColor(QColor(Qt::white));
        pen.setWidth(2);
        symbol->setPen(pen);

        marker->setSymbol(symbol);
        marker->setValue(curve->sample(rowIndex));
        marker->attach(this);
        ++markerCount;
        qDebug()<<"Marker appended. Total:"<<markerCount;
    }

}

void Plot::selectT0(bool on) {
    if (on) {
    selectPointsMode(false);
    selectPointPicker->setEnabled(true);
    connect(this,SIGNAL(curvePointClicked(QwtPlotCurve*,int)),SLOT(setT0(QwtPlotCurve*,int)));

    zoomerLeft->setEnabled(false);
    zoomerRight->setEnabled(false);
    zoomerExclusiveLeft->setEnabled(false);
    zoomerExclusiveRight->setEnabled(false);

    emit message("Select T0");
}

    if (!on) {
        selectPointsMode(false);
        disconnect(this,SLOT(setT0(QwtPlotCurve*,int)));
        return;
    }
}

QVector<QPointF> Plot::getSelectedPoints(QwtPlotCurve *curve) const
{
    QwtPlotCurve *selectionCurve;
    if(curve!=NULL)  {
        if (markCurveMap.contains(curve))
        selectionCurve=markCurveMap[curve];
    } else {
        if (markCurveMap.contains(selectedCurve))
        selectionCurve=markCurveMap[selectedCurve];
    }
    if (!selectionCurve) {
        qWarning()<<"Failed to get selection curve";
        return QVector<QPointF> ();
    }

    // form an array of QPointF
    if (!dataMap.contains(selectionCurve)) {
        qWarning("No points selected");
        return QVector<QPointF> ();
    }
    return dataMap[selectionCurve];
}

bool Plot::hasSelectedPoints() const
{
    if (selectedCurve!=NULL) return true;
    return false;
}

//Returns x value (time) for selected T0 point
double Plot::getT0() const
{
    //if T0 is not selected return 0;
    if (T0index<0) return 0;


    QwtPlotCurve *curve;
    const QwtPlotItemList& itmList = itemList(QwtPlotItem::Rtti_PlotCurve);
    //find first curve that is on legend
    for (QwtPlotItemIterator it = itmList.begin(); it != itmList.end(); ++it ) {
        curve=(QwtPlotCurve*)(*it);
        if (!curve->testItemAttribute(QwtPlotItem::Legend)) {
            continue;
        }
        break;
    }

    return curve->sample(T0index).x();
}

double Plot::getT0Value(QwtPlotCurve *curve) const
{
    if (curve==NULL) {
        qWarning()<<"Curve not selected";
        return 0;
    } else if (T0index<0) {
        qWarning()<<"T0 not selected";
        return 0;
    }

    return curve->sample(T0index).y();
}

bool Plot::issetT0() const
{
    if (T0index<0) return false;
    else return true;
}

void Plot::setT0(QwtPlotCurve *curve, int index)
{
    if (!curve) {
        T0index=-1;
        return;
    }

    T0index=index;
    emit T0Selected();
}

void Plot::addInterpolationCurve(const QVector<QPointF> &points, QwtPlotCurve * originalDataCurve)
{
    if (points.size()<=0) {
        qWarning()<<"No points to add interpolation curve";
        return;
    }

    QwtPlotCurve *interpolationCurve=new QwtPlotCurve(QString::number(points.first().x())+" "+interpolationCurve->title().text());
    interpolationCurve->setItemAttribute(QwtPlotItem::Legend,false);
    interpolationCurve->setSamples(points);
    interpolationCurve->setYAxis(originalDataCurve->yAxis());
    interpolationCurve->setVisible(showInterpolation);
    interpolationCurve->attach(this);
    approximationCurveList.append(interpolationCurve);
    QwtPlot::replot();
}

void Plot::showApproximationCurves(bool show)
{
    showInterpolation=show;
    for (int i=0;i<approximationCurveList.size();++i) {
        approximationCurveList[i]->setVisible(show);
    }
    QwtPlot::replot();
}
