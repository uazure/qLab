#ifndef PLOT_H
#define PLOT_H
#include "qwt_plot.h"
#include "seriesdata.h"
#include "experimentdata.h"
#include "qwt_plot_curve.h"
#include "qwt_symbol.h"
#include "qwt_plot_grid.h"

class Plot : public QwtPlot
{
    Q_OBJECT
    //unidentified copy constructor
    Plot(const Plot&);
public:
    explicit Plot(QWidget *parent, ExperimentData* data);
    ~Plot();
    void addCurve(int yColumn, int xColumn=-1);


private:
    const ExperimentData *experimentData;
    const QVector<QVector<double> > *dataTable;
    int xCol;


signals:

public slots:
    void clear(void);
    void initialize(void);

};

#endif // PLOT_H
