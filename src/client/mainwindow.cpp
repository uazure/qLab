#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    plot=new Plot(this);
    data=new ExperimentData(this);



    ui->plotLayout->addWidget(plot);

    connect(ui->actionFullscreen,SIGNAL(triggered(bool)),this,SLOT(setFullscreen(bool)));
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionConnect_to,SIGNAL(triggered()),this,SLOT(connectTo()));
    connect(ui->actionDisconnect,SIGNAL(triggered()),this,SLOT(socketDisconnect()));
    connect(ui->actionViewData,SIGNAL(triggered()),this,SLOT(showDataViewWindow()));
    connect(&tcpClient,SIGNAL(connected()),this,SLOT(socketConnectedToServer()));
    connect(&tcpClient,SIGNAL(disconnected()),this,SLOT(socketDisconnectedFromServer()));
    connect(&tcpClient,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    connect(&tcpClient,SIGNAL(dataLine(QByteArray&)),data,SLOT(parseLine(QByteArray&)));
    connect(&tcpClient,SIGNAL(initialData()),data,SLOT(resetData()));


    connectionLabel.setText("*");
    connectionLabel.setStyleSheet("QLabel {color:red; font-weight:bold;}");
    ui->statusBar->addPermanentWidget(&connectionLabel);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete plot;
    delete data;
}

void MainWindow::setFullscreen(bool a) {
    if (a)
     setWindowState(Qt::WindowFullScreen);
    else
     setWindowState(windowState() ^ Qt::WindowFullScreen);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (event->spontaneous()) event->accept();
    QMessageBox dialog(this);
    dialog.setText("Really exit?");
    dialog.setIcon(QMessageBox::Question);
    dialog.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    dialog.setDefaultButton(QMessageBox::Cancel);
    dialog.setFocus();

    if (dialog.exec()==QMessageBox::Ok)
    {

    //FIXME: Here should be some actions to do befor closing app
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::connectTo() {
    bool ok;
    QString hostname=QInputDialog::getText(this,"Specify host name or ip","Host to connect to",QLineEdit::Normal,QString(),&ok,0);
    if (ok) {
        qDebug()<<"Connecting to"<<hostname<<"port"<<25050;
        this->tcpClient.connectToHost(hostname,25050);
    }

}
void MainWindow::socketConnectedToServer() {
    qDebug()<<"Connected!";
    ui->actionDisconnect->setDisabled(false);
    connectionLabel.setStyleSheet("QLabel {color:green;}");
}

void MainWindow::socketDisconnectedFromServer() {
    qWarning()<<"Disconnected from server!";
    QMessageBox msgBox(this);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setText("Disconnected from server");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.exec();
}

void MainWindow::socketStateChanged(QAbstractSocket::SocketState state) {
    switch (state) {
    case QAbstractSocket::UnconnectedState:
        qWarning()<<"Unconnected";
//        QMessageBox msgBox(this);
//        msgBox.setStandardButtons(QMessageBox::Ok);
//        msgBox.setText("Failed to connect to server");
//        msgBox.setIcon(QMessageBox::Warning);
//        msgBox.show();
        break;
    case QAbstractSocket::HostLookupState:
        qDebug()<<"Lookup for host";
        break;
    case QAbstractSocket::ConnectingState:
        qDebug()<<"Connecting";
        break;
    case QAbstractSocket::ConnectedState:
        qDebug()<<"Connected!";
        break;
    case QAbstractSocket::ClosingState:
        qWarning()<<"Closing socket!";
        break;
    }
}

void MainWindow::socketDisconnect(void) {
    tcpClient.disconnectFromHost();
    ui->actionDisconnect->setDisabled(true);
}

void MainWindow::showDataViewWindow() {
    DataViewWindow *dataView=new DataViewWindow(this);
    dataView->setAscii(data->getAscii());
    dataView->setItem(data);
    dataView->show();
}
