#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QCloseEvent>
#include <QLabel>

#include "plot.h"
#include "qwt_plot_curve.h"
#include "qwt_series_data.h"
#include "qwt_symbol.h"
#include "seriesdata.h"
#include "tcpclient.h"
#include "experimentdata.h"
#include "dataviewwindow.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    //unidentified copy constructor
    MainWindow(const MainWindow &);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Plot *plot;
    ExperimentData *data;
    QwtPlotCurve * plotCurve;
//    QwtPlotCurve * plotCurve2;
    SeriesData *series;
//    SeriesData *series2;
    TcpClient tcpClient;
    void closeEvent(QCloseEvent *);
    QLabel connectionLabel;


private slots:
    void setFullscreen(bool fullscreen);
    void connectTo(void);
    void socketConnectedToServer(void);
    void socketDisconnectedFromServer(void);
    void socketStateChanged(QAbstractSocket::SocketState state);
    void socketDisconnect(void);
    void showDataViewWindow(void);

};

#endif // MAINWINDOW_H
