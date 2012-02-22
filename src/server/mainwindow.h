#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "devicefarm.h"
#include "experimentconfigurationselectdialog.h"
#include "controlmanagementdialog.h"
#include "experiment.h"
#include "tcpserver.h"
#include "udpserver.h"

#include <QSettings>
#include <QLabel>
#include <QLocale>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);
    Experiment * getExperimentInstance(void) const;

private:
    Ui::MainWindow *ui;
    QSettings *experimentSettings;
    Experiment *experiment;
    TcpServer *tcp;
    UdpServer *udp;
    QLabel experimentLabel;
    QLabel fileLabel;
    QLabel statusLabel;
    QLabel tcpClientsLabel;
    QStringList devices;
    QList <QStringList> parameters;
    QList <AbstractDevice *> deviceList;
    void initDevices();

private slots:
    void setFullscreen(bool isFullscreen);
    void getExperiment();
    void showExperimentControlMangement();
    void showAboutQt();

public slots:
    void setExperiment(QString experiment);
    void setFile(QString filename);
    void getFile(void);
    void Notify(QString message);
    void statusChanged(bool status);
    //experiment can set ui to display interval
    void setMeasureInterval(int msec);
    void changeMeasureIntervalDialog();

};

#endif // MAINWINDOW_H
