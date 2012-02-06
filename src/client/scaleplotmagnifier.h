#ifndef SCALEPLOTMAGNIFIER_H
#define SCALEPLOTMAGNIFIER_H

#include "scalemagnifier.h"
//#include "qwt_plot_magnifier.h"
#include "qwt_plot.h"
#include "qwt_plot_canvas.h"
#include "qwt_scale_div.h"


class ScalePlotMagnifier : public ScaleMagnifier
{
    Q_OBJECT

public:
    explicit ScalePlotMagnifier(QwtPlotCanvas *canvas);
    virtual ~ScalePlotMagnifier();

    void setAxisEnabled( int axis, bool on );
    bool isAxisEnabled( int axis ) const;

    QwtPlotCanvas *canvas();
    const QwtPlotCanvas *canvas() const;

    QwtPlot *plot();
    const QwtPlot *plot() const;

protected:
    virtual void rescale( double factor );

private:
    class PrivateData;
    PrivateData *d_data;
};

#endif // SCALEPLOTMAGNIFIER_H
