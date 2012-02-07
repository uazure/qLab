#ifndef SETUPCURVESDIALOG_H
#define SETUPCURVESDIALOG_H

#include <QDialog>
#include <QAbstractItemModel>
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
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

private:
    Ui::SetupCurvesDialog *ui;
    QwtPlot *plot;
    ExperimentData *data;
};

#endif // SETUPCURVESDIALOG_H
