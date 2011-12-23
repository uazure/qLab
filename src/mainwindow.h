#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "devicefarm.h"
#include "experimentconfigurationselectdialog.h"
#include <QSettings>
#include <QLabel>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSettings *experimentSettings;
    QLabel experimentLabel;
    QString experiment;
    QStringList devices;
    QList <QStringList> parameters;
    void initDevices();
    QList <QAbstractDevice *> deviceList;

private slots:
    void setFullscreen(bool isFullscreen);
    void getExperiment();
    void showAboutQt(void);

public slots:
    void setExperiment(QString experiment);

};

#endif // MAINWINDOW_H
