#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QCloseEvent>
#include <QLabel>

#include "plot.h"
#include "tcpclient.h"
#include "experimentdata.h"

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
    Plot *plot;
    ExperimentData *data;
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

};

#endif // MAINWINDOW_H
