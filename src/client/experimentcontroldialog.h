#ifndef EXPERIMENTCONTROLDIALOG_H
#define EXPERIMENTCONTROLDIALOG_H

#include <QDialog>
#include <QInputDialog>
#include "experiment.h"
#include "tcpclient.h"

namespace Ui {
    class ExperimentControlDialog;
}

class ExperimentControlDialog : public QDialog {
    Q_OBJECT
public:
    ExperimentControlDialog(Experiment *experimentPointer, TcpClient *tcpClientPointer,QWidget *parent = 0);
    ~ExperimentControlDialog();


protected:
    void changeEvent(QEvent *e);

private:
    Ui::ExperimentControlDialog *ui;
    TcpClient *tcpClient;
    Experiment *experiment;

private slots:
    void controlIndexChanged(int index);
    void setControlTarget(int control,QString value);
    void setControlTarget(void);

};

#endif // EXPERIMENTCONTROLDIALOG_H
