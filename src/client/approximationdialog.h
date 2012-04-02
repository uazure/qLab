#ifndef APPROXIMATIONDIALOG_H
#define APPROXIMATIONDIALOG_H

#include <QDialog>
#include "plot.h"

namespace Ui {
    class ApproximationDialog;
}

class ApproximationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ApproximationDialog(Plot *plotWidget, QWidget *parent=NULL);
    ~ApproximationDialog();

    int getTApproximationMethodId() const;
    int getFApproximationMethodId() const;
    QwtPlotCurve * getTCurve() const {return tCurve;}
    QwtPlotCurve * getFCurve() const {return fCurve;}
    static QwtPlotCurve *lastTCurve, *lastFCurve;
    static int lastTApproximationMethod, lastFApproximationMethod;



private slots:
    void TCurveSelected(int index) {tCurve=curveList.at(index);
                                   lastTCurve=tCurve;}
    void FCurveSelected(int index) {fCurve=curveList.at(index);
                                   lastFCurve=fCurve;}

private:
    Ui::ApproximationDialog *ui;
    Plot *plot;
    QList<QwtPlotCurve *> curveList;
    QList<QString> curveNameList;
    QwtPlotCurve *tCurve,*fCurve;
};

#endif // APPROXIMATIONDIALOG_H
