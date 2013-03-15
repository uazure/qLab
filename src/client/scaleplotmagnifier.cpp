
/* Used code from

 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/


#include "scaleplotmagnifier.h"

class ScalePlotMagnifier::PrivateData
{
public:
    PrivateData()
    {
        for ( int axis = 0; axis < QwtPlot::axisCnt; axis++ )
            isAxisEnabled[axis] = true;
    }

    bool isAxisEnabled[QwtPlot::axisCnt];
};



ScalePlotMagnifier::ScalePlotMagnifier(QwtPlotCanvas *canvas) :
    ScaleMagnifier(canvas)
{
    d_data= new PrivateData();
}


ScalePlotMagnifier::~ScalePlotMagnifier() {
    delete d_data;
}

void ScalePlotMagnifier::setAxisEnabled( int axis, bool on )
{
    if ( axis >= 0 && axis < QwtPlot::axisCnt )
        d_data->isAxisEnabled[axis] = on;
}

bool ScalePlotMagnifier::isAxisEnabled( int axis ) const
{
    if ( axis >= 0 && axis < QwtPlot::axisCnt )
        return d_data->isAxisEnabled[axis];

    return true;
}

QwtPlotCanvas *ScalePlotMagnifier::canvas()
{
    return qobject_cast<QwtPlotCanvas *>( parent() );
}

const QwtPlotCanvas *ScalePlotMagnifier::canvas() const
{
    return qobject_cast<const QwtPlotCanvas *>( parent() );
}

QwtPlot *ScalePlotMagnifier::plot()
{
    QwtPlotCanvas *w = canvas();
    if ( w )
        return w->plot();
    return NULL;
}

const QwtPlot *ScalePlotMagnifier::plot() const
{
    const QwtPlotCanvas *w = canvas();
    if ( w )
        return w->plot();

    return NULL;
}

void ScalePlotMagnifier::rescale( double factor )
{
    factor = qAbs( factor );
    if ( factor == 1.0 || factor == 0.0 )
        return;

    bool doReplot = false;
    QwtPlot* plt = plot();

    const bool autoReplot = plt->autoReplot();
    plt->setAutoReplot( false );

    for ( int axisId = 0; axisId < QwtPlot::axisCnt; axisId++ )
    {
        const QwtScaleDiv *scaleDiv = &plt->axisScaleDiv( axisId );
        if ( isAxisEnabled( axisId ) /*&& scaleDiv->isValid()*/ )
        {
            const double center =
                scaleDiv->lowerBound() + scaleDiv->range() / 2;
            const double width_2 = scaleDiv->range() / 2 * factor;

            plt->setAxisScale( axisId, center - width_2, center + width_2 );
            doReplot = true;
        }
    }

    plt->setAutoReplot( autoReplot );

    if ( doReplot )
        plt->replot();
}
