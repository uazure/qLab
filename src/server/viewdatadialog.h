#ifndef VIEWDATADIALOG_H
#define VIEWDATADIALOG_H

#include "experiment.h"
#include <QDialog>

namespace Ui {
    class ViewDataDialog;
}

class ViewDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewDataDialog(Experiment *experimentObject, QWidget *parent = 0);
    ~ViewDataDialog();

private:
    Ui::ViewDataDialog *ui;
    Experiment *experiment;
};

#endif // VIEWDATADIALOG_H
