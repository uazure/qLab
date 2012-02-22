#ifndef CONTROLMANAGEMENTDIALOG_H
#define CONTROLMANAGEMENTDIALOG_H

#include <QDialog>
#include "experiment.h"
#include "abstractthermocontrollerdevice.h"
#include "abstractdevice.h"

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

private slots:
    void updateControlInfo(int index);
    void changeTarget(int controlIndex);
};

#endif // CONTROLMANAGEMENTDIALOG_H
