#ifndef SETUPCURVESDIALOG_H
#define SETUPCURVESDIALOG_H

#include <QDialog>
#include <QAbstractItemModel>
#include <QList>
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_symbol.h"
#include "ColorBox.h"
#include "experimentdata.h"



namespace Ui {
    class SetupCurvesDialog;
}

class SetupCurvesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetupCurvesDialog(QWidget *parent, QwtPlot *plot, ExperimentData *experimentData);
    ~SetupCurvesDialog();

public slots:
    void curveSelected(int index);
    void setCurveColor(void);
    void setCurveWidth(double width);
    void setCurveLine(bool on);
    void setCurveSymbolSize(int size);

private:
    Ui::SetupCurvesDialog *ui;
    QwtPlot *plot;
    ExperimentData *data;
    QList<QwtPlotCurve *> curveList;
    QwtPlotCurve *currentCurve;
    ColorBox *colorSelect;

};

#endif // SETUPCURVESDIALOG_H
