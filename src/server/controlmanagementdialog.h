#ifndef CONTROLMANAGEMENTDIALOG_H
#define CONTROLMANAGEMENTDIALOG_H

#include <QDialog>
#include "experiment.h"

namespace Ui {
    class ControlManagementDialog;
}

class ControlManagementDialog : public QDialog {
    Q_OBJECT
public:
    ControlManagementDialog(Experiment *experiment,QWidget *parent = 0);
    ~ControlManagementDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ControlManagementDialog *ui;
    Experiment *experiment;
};

#endif // CONTROLMANAGEMENTDIALOG_H
