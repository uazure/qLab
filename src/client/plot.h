#ifndef PLOT_H
#define PLOT_H
#include "qwt_plot.h"
#include "seriesdata.h"
#include "experimentdata.h"
#include "qwt_plot_curve.h"
#include "qwt_symbol.h"
#include "qwt_plot_grid.h"
#include "qwt_legend.h"
#include "qwt_plot_picker.h"
#include "qwt_picker_machine.h"
#include "qwt_plot_directpainter.h"

#include <QDebug>

class Plot : public QwtPlot
{
    Q_OBJECT
    //unidentified copy constructor
    Plot(const Plot&);
public:
    explicit Plot(QWidget *parent, ExperimentData* data);
    ~Plot();
    void addCurve(int yColumn, int xColumn=-1);

public slots:
    void replot();


private:
    const ExperimentData *experimentData;
    const QVector<QVector<double> > *dataTable;
    int xCol;
    QwtPlotCurve *selectedCurve;
    int selectedPoint;

private slots:
    void hidePlotItem(QwtPlotItem *plotItem, bool hide);
    void getSelectedPoints(const QVector<QPointF> &points);
    void getSelectedPoints(const QPointF &point);
    void getSelectedCanvasPoints(const QPoint &point);
    void clearPointSelection();

    /** Slot is called when user selects some point on the curve. If to
      is omitted then it means that only ONE curve point should be marked,
i.e. from=to */
    void markCurvePoint(QwtPlotCurve *curve,int from,int to=-1);
    void unmarkCurvePoint(QwtPlotCurve *curve,int from,int to=-1);



signals:
    void curvePointClicked(QwtPlotCurve *curve,int index);

public slots:
    void clear(void);
    void initialize(void);

};

#endif // PLOT_H
