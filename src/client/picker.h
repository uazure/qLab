#ifndef PICKER_H
#define PICKER_H
#include "qwt_picker.h"
#include "qwt_plot_curve.h"
#include "qwt_plot.h"
#include "qwt_plot_canvas.h"
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>

class Picker : public QwtPicker
{
    Q_OBJECT
public:
    explicit Picker(QwtPlot* plot);

    virtual bool eventFilter(QObject *obj, QEvent *e);
    virtual bool event(QEvent *e);


    QwtPlot *plot() { return (QwtPlot *)parent(); }
    const QwtPlot *plot() const {return (QwtPlot *)parent();}

private:
    QwtPlotCurve *selectedCurve;
    int selectedPoint;




signals:
    void selectSignle(QPoint pos);
    void selectRange(QPoint pos);
    void appendSingle(QPoint pos);

public slots:

};

#endif // PICKER_H
