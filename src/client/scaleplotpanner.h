#ifndef SCALEPLOTPANNER_H
#define SCALEPLOTPANNER_H

#include "qwt_plot_panner.h"
#include "qwt_plot_canvas.h"
#include <QEvent>
#include <QWidget>

class ScalePlotPanner:public QwtPlotPanner
{
    Q_OBJECT

public:
    ScalePlotPanner(QwtPlotCanvas * canvas);
    bool eventFilter(QObject *object, QEvent *event);
};

#endif // SCALEPLOTPANNER_H
