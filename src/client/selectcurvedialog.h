#ifndef SELECTCURVEDIALOG_H
#define SELECTCURVEDIALOG_H

#include <QDialog>

class Plot;
class QwtPlotCurve;


namespace Ui {
    class selectCurveDialog;
}

class SelectCurveDialog : public QDialog {
    Q_OBJECT
public:
    SelectCurveDialog(const QString &itemName, Plot *plot);
    ~SelectCurveDialog();
    QwtPlotCurve * getSelectedCurve() const;
    bool isRememberChecked() const;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::selectCurveDialog *ui;
    QList<QwtPlotCurve*> curveList;
    QStringList curveNameList;
    QwtPlotCurve *selectedCurve;

private slots:
    void curveSelected(int index);


};

#endif // SELECTCURVEDIALOG_H
