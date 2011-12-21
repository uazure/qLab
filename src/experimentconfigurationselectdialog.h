#ifndef EXPERIMENTCONFIGURATIONSELECTDIALOG_H
#define EXPERIMENTCONFIGURATIONSELECTDIALOG_H

#include <QDialog>

namespace Ui {
    class ExperimentConfigurationSelectDialog;
}

class ExperimentConfigurationSelectDialog : public QDialog {
    Q_OBJECT
public:
    ExperimentConfigurationSelectDialog(QWidget *parent = 0);
    ~ExperimentConfigurationSelectDialog();

    void setExperimentList(QStringList &list);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ExperimentConfigurationSelectDialog *ui;
};

#endif // EXPERIMENTCONFIGURATIONSELECTDIALOG_H
