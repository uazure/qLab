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
    //updates ui with target for specified control
    void setControlTarget(int control,QString value);
    //shows dialog for setting new target for current control
    void setControlTarget(void);

    void setHistory(QStringList history);

};

#endif // EXPERIMENTCONTROLDIALOG_H
