#ifndef SETUPCURVESDIALOG_H
#define SETUPCURVESDIALOG_H

#include <QDialog>
#include <QAbstractItemModel>
#include <QList>
#include <QInputDialog>
#include "plot.h"
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
    explicit SetupCurvesDialog(QWidget *parent, Plot *plot, ExperimentData *experimentData);
    ~SetupCurvesDialog();

public slots:
    void curveSelected(int index);
    void setCurveColor(void);
    void setCurveWidth(double width);
    void setCurveLine(bool on);
    void setCurveSymbolSize(int size);
    void setCurveYAxis(int index);
    void deleteCurrentCurve();
    void addCurveDialog();

private:
    Ui::SetupCurvesDialog *ui;
    Plot *plot;
    ExperimentData *data;
    QList<QwtPlotCurve *> curveList;
    QwtPlotCurve *currentCurve;
    ColorBox *colorSelect;
    void init(void);

};

#endif // SETUPCURVESDIALOG_H
