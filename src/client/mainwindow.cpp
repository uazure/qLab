#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "nonlinearapproximation.h"
#include "basicconfigurationdialog.h"

MainWindow::MainWindow(QString filename, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    appSettings=new QSettings(QSettings::IniFormat,QSettings::UserScope,QApplication::organizationName(),QApplication::applicationName(),this);

    data=new ExperimentData(this);
    dilatometerData=new DilatometerData(this);
    experiment=new Experiment(&tcpClient,this);
    plot=new Plot(this,data);
    plot->setAxisTitle(QwtPlot::xBottom,tr("t, sec"));


    ui->plotLayout->addWidget(plot);

    connect(ui->actionFullscreen,SIGNAL(triggered(bool)),this,SLOT(setFullscreen(bool)));
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionConnect_to,SIGNAL(triggered()),this,SLOT(connectTo()));
    connect(ui->actionDisconnect,SIGNAL(triggered()),this,SLOT(socketDisconnect()));
    connect(ui->actionViewData,SIGNAL(triggered()),this,SLOT(showDataViewWindow()));
    connect(ui->actionReplot,SIGNAL(triggered()),plot,SLOT(replot()));
    connect(ui->actionClear_plot,SIGNAL(triggered()),plot,SLOT(clear()));
    connect(ui->actionInitialize,SIGNAL(triggered()),plot,SLOT(initialize()));
    connect(ui->actionSelect_points,SIGNAL(toggled(bool)),plot,SLOT(selectPointsMode(bool)));
    connect(ui->actionZoom_to_extents,SIGNAL(triggered()),plot,SLOT(zoomExtents()));
    connect(ui->actionDraw_incremental,SIGNAL(triggered(bool)),plot,SLOT(setIncrementalDraw(bool)));
    connect(ui->actionOpen_file,SIGNAL(triggered()),SLOT(openFile()));
    connect(ui->actionSave_as,SIGNAL(triggered()),SLOT(saveFile()));
    connect(ui->actionMonitoring_interval,SIGNAL(triggered()),SLOT(setMonitoringInterval()));
    connect(ui->actionSetup,SIGNAL(triggered()),SLOT(showSetupCurvesDialog()));
    connect(ui->actionSet_interval,SIGNAL(triggered()),SLOT(setInterval()));
    connect(ui->actionControl,SIGNAL(triggered()),SLOT(viewExperimentControlDialog()));
    connect(ui->actionStart,SIGNAL(triggered(bool)),&tcpClient,SLOT(start(bool)));

    connect(ui->actionApproximate,SIGNAL(triggered()),SLOT(approximate()));
    connect(ui->actionShow_approximations,SIGNAL(triggered(bool)),plot,SLOT(showApproximationCurves(bool)));
    connect(ui->actionConfiguration,SIGNAL(triggered()),SLOT(showConfigurationDialog()));
    connect(ui->actionLast_calculation,SIGNAL(triggered()),SLOT(showCalculationLog()));

    ui->actionDraw_incremental->trigger();

    connect(plot,SIGNAL(message(QString)),statusBar(),SLOT(showMessage(QString)));
    connect(ui->actionSelectT0,SIGNAL(toggled(bool)),plot,SLOT(selectT0(bool)));
    connect(plot,SIGNAL(T0Selected(bool)),ui->actionSelectT0,SLOT(trigger()));
    connect(plot,SIGNAL(T0Selected(bool)),ui->actionSelect_points,SLOT(setChecked(bool)));

    connect(&tcpClient,SIGNAL(connected()),this,SLOT(socketConnectedToServer()));
    connect(&tcpClient,SIGNAL(disconnected()),this,SLOT(socketDisconnectedFromServer()));
    connect(&tcpClient,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    connect(&tcpClient,SIGNAL(dataLine(QByteArray&)),data,SLOT(parseLine(QByteArray&)));
    connect(&tcpClient,SIGNAL(initialData()),plot,SLOT(clear()));
    connect(&tcpClient,SIGNAL(initialData()),data,SLOT(resetData()));
    connect(&tcpClient,SIGNAL(serverStatus(bool)),ui->actionStart,SLOT(setChecked(bool)));

//FIXME: al least bytesRead indicates not the total number of bytes that was read from network. signalled value should be added to current number of bytes read
    connect(&tcpClient,SIGNAL(bytesWritten(int)),&bytesWrittenLabel,SLOT(setNum(int)));
    connect(&tcpClient,SIGNAL(bytesRead(int)),&bytesReadLabel,SLOT(setNum(int)));
    connect(&tcpClient,SIGNAL(serverInterval(int)),experiment,SLOT(setInterval(int)));


    connect(data,SIGNAL(initialized()),plot,SLOT(initialize()));
    connect(data,SIGNAL(pointCount(int)),&pointCountLabel,SLOT(setNum(int)));
    connect(data,SIGNAL(pointCount(int)),plot,SLOT(drawLastPoint(int)));
    /** This connection will cause to plot VLine markers*/
    //connect(data,SIGNAL(marker(QPointF,int)),plot,SLOT(appendMarker(QPointF,int)));
    connect(data,SIGNAL(marker(int)),plot,SLOT(appendMarker(int)));



    connectionLabel.setText("*");
    connectionLabel.setToolTip(tr("Connection status:\nGreen - connected\nRed - disconnected"));
    connectionLabel.setStyleSheet("QLabel {color:red; font-weight:bold;}");
    pointCountLabel.setText("0");
    pointCountLabel.setToolTip(tr("Number of points"));
    bytesWrittenLabel.setText("0");
    bytesWrittenLabel.setToolTip("Bytes written to network");
    bytesReadLabel.setText("0");
    bytesReadLabel.setToolTip("Bytes read from network");

    ui->statusBar->addPermanentWidget(&bytesReadLabel);
    ui->statusBar->addPermanentWidget(&bytesWrittenLabel);
    ui->statusBar->addPermanentWidget(&pointCountLabel);
    ui->statusBar->addPermanentWidget(&connectionLabel);

    //if file was specified on startup - try open it
    if (!filename.isEmpty()) openFile(filename);


}

MainWindow::~MainWindow()
{
    plot->setVisible(false);
    delete plot;
    delete data;
    delete dilatometerData;
    delete appSettings;
    delete ui;

}

void MainWindow::setFullscreen(bool a) {
    if (a) {
        setWindowState(Qt::WindowFullScreen);
    } else {
        setWindowState(windowState() ^ Qt::WindowFullScreen);
    }
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
    QString hostname=QInputDialog::getText(this,"Specify host name or ip","Host to connect to",QLineEdit::Normal,appSettings->value("Connect to").toString(),&ok,0);
    if (ok) {
        appSettings->setValue("Connect to",QVariant(hostname));
        qDebug()<<"Connecting to"<<hostname<<"port"<<25050;
        this->tcpClient.connectToHost(hostname,25050);
    }

}
void MainWindow::socketConnectedToServer() {
    qDebug()<<"Connected!";
    ui->actionDisconnect->setDisabled(false);
    ui->menuExperiment->setEnabled(true);
    connectionLabel.setStyleSheet("QLabel {color:green;font-weight:bold;}");
}

void MainWindow::socketDisconnectedFromServer() {
    qWarning()<<"Disconnected from server!";
    ui->menuExperiment->setEnabled(false);
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
    case QAbstractSocket::BoundState:
        qDebug()<<"Socket is bound";
        break;
    case QAbstractSocket::ListeningState:
        qDebug()<<"Socket is listening";
        break;
    }
}

void MainWindow::socketDisconnect(void) {
    tcpClient.disconnectFromHost();
    ui->actionDisconnect->setDisabled(true);
    connectionLabel.setStyleSheet("QLabel {color:red;font-weight:bold;}");
}

void MainWindow::showDataViewWindow() {
    DataViewWindow *dataView=new DataViewWindow(this);
    dataView->setAscii(data->getAscii());
    dataView->setItem(data);
    dataView->show();
}


void MainWindow::openFile(QString filename) {
    if (filename.isEmpty()) {
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::ExistingFile);
        QStringList nameFilters, fileNames;
        nameFilters.append(tr("Data files (*.dat *.txt)"));
        nameFilters.append(tr("All files (*)"));
        dialog.setNameFilters(nameFilters);
        if (!dialog.exec()) {
            qWarning()<<"No file has been selected";
            return;
        }
        fileNames=dialog.selectedFiles();
        if (fileNames.isEmpty()) return;
        filename=fileNames.at(0);
    }

    data->readFile(filename);
}

void MainWindow::saveFile(QString filename) {
    if (filename.isEmpty()) {
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::AnyFile);
        QStringList nameFilters, fileNames;
        nameFilters.append(tr("Data files (*.dat *.txt)"));
        nameFilters.append(tr("All files (*)"));
        dialog.setNameFilters(nameFilters);
        if (!dialog.exec()) {
            qWarning()<<"No file has been selected";
            return;
        }
        fileNames=dialog.selectedFiles();
        if (fileNames.isEmpty()) return;
        filename=fileNames.at(0);
    }

    data->saveFile(filename);
}

void MainWindow::setMonitoringInterval(void)
{
    bool ok;
    double interval = QInputDialog::getDouble(this, tr("Input monitoring interval in sec"),
                                        tr("Monitoring interval:"),plot->getMonitoringInterval()/60,0.5,1440,1, &ok);
    if (ok) {
        plot->setMonitoringInterval(interval*60);
        qDebug()<<"Monitoring interval set to"<<interval;
    } else {
        qWarning()<<"Monitoring interval not set";
    }
}

void MainWindow::showSetupCurvesDialog(void)
{
    SetupCurvesDialog * dialog=new SetupCurvesDialog(this,plot,data);
    dialog->exec();
    delete dialog;
}


void MainWindow::setInterval() {
    if (!experiment->getInterval()) {
        qDebug()<<"No measuring interval set. Asking server";
        connect(&tcpClient,SIGNAL(serverInterval(int)),SLOT(setInterval()));
        tcpClient.query(TcpClient::queryInterval);
        return;
    }

    disconnect(&tcpClient,SIGNAL(serverInterval(int)),this,SLOT(setInterval()));

    bool ok=false;
    double currentValue=experiment->getInterval();
    currentValue=currentValue/1000;

    double interval=QInputDialog::getDouble(this,tr("Input experiment measure interval in sec"),tr("Measure interval, sec"),currentValue,0.1,3600,2,&ok);
    if (!ok) {
        return;
    }
    tcpClient.setMeasureInterval((int) interval*1000);
}

void MainWindow::viewExperimentControlDialog() {
    ExperimentControlDialog *dialog=new ExperimentControlDialog(experiment,&tcpClient,this);
    dialog->exec();
    delete dialog;
}

void MainWindow::approximate(void)
{
    if (!plot->issetT0()) {
        QMessageBox::warning(this,tr("T0 not selected"),tr("Please, select T0 and range of points"));
        return;
    }
    if (!plot->hasSelectedPoints()) {
        QMessageBox::warning(this,tr("No points selected"),tr("Please, select range of points"));
        return;
    }

    //show dialog for selecting curve and approximation method for Temperature and Pressure
    ApproximationDialog dialog(plot,this);
    if (!dialog.exec()) {
        qWarning()<<"Curves and approximation method has not been chosen";
        return;
    }
    QwtPlotCurve *TCurve=dialog.getTCurve();
    QwtPlotCurve *FCurve=dialog.getFCurve();
    int TapproximationMethod=dialog.getTApproximationMethodId();
    int FapproximationMethod=dialog.getFApproximationMethodId();


    QVector<QPointF> Tpoints=plot->getSelectedPoints(TCurve);
    QVector<QPointF> Fpoints=plot->getSelectedPoints(FCurve);

    double x0=plot->getT0();
    for (int i=0;i<Tpoints.size();++i) {
        Tpoints[i].setX(Tpoints.at(i).x()-x0);
        Fpoints[i].setX(Fpoints.at(i).x()-x0);
    }




    NonLinearApproximation approximationT, approximationF;
    qDebug()<<"Starting solver";
    QString Tlog,Flog;
    QVector<double> paramsT, paramsF;

    int resultT=approximationT.solve(Tpoints,TapproximationMethod,Tlog,paramsT);
    int resultF=approximationF.solve(Fpoints,FapproximationMethod,Flog,paramsF);

    dilatometerData->setLog(QString("Temperature:\n")+Tlog+"\n\nFrequency:\n"+Flog);

    if (resultT==GSL_SUCCESS && resultF==GSL_SUCCESS) {
        QMessageBox::information(this,tr("Information"),dilatometerData->getLog());
    } else {
        int result = QMessageBox::warning(this,tr("Warning"),dilatometerData->getLog(),QMessageBox::Ok,QMessageBox::Cancel);
        if (result==QMessageBox::Cancel) {
            return;
        }
    }




    qDebug()<<"Creting interpolation curves";
    //create interpolation curves and add them to plot
    QVector<QPointF> TinterpolationPoints=approximationT.getApproximationPoints();
    QVector<QPointF> FinterpolationPoints=approximationF.getApproximationPoints();

    //add x0 to interpolation results
    for (int i=0;i<TinterpolationPoints.size();++i) {
        TinterpolationPoints[i].setX(TinterpolationPoints.at(i).x()+x0);
    }

    for (int i=0;i<FinterpolationPoints.size();++i) {
        FinterpolationPoints[i].setX(FinterpolationPoints.at(i).x()+x0);
    }

    //add approximation curves to the plot
    if (TinterpolationPoints.size()>0 && FinterpolationPoints.size()>0) {
        qDebug()<<"Adding interpolation curves to plot";
        plot->addApproximationCurve(TinterpolationPoints,TCurve);
        plot->addApproximationCurve(FinterpolationPoints,FCurve);
    } else {
        qDebug()<<"interploation curves are empty. Exiting";
        return;
    }

    //calculate dT, dF and create thermal expansion point
    double dT,T0,T1;
    bool onHeat=true;

    qDebug()<<"Calculating T0 and T1";
    switch (TapproximationMethod) {
    case NonLinearApproximation::methodTailAvg:
        T0=TCurve->sample(plot->getT0Index()).y();
        T1=paramsT.first();
        break;
    case NonLinearApproximation::methodExp:
        T0=TCurve->sample(plot->getT0Index()).y();
        T1=paramsT.first();
        break;
    default:
        T0=TCurve->sample(plot->getT0Index()).y();
        T1=T0+1;
        QMessageBox::warning(this,tr("deltaT can't be calculated"),tr("Temperature approximation method can not be used for deltaT calculation"));
        return;
        break;
    }

    //determine dT and onHeat
    dT=T1-T0;
    if (T1>T0) onHeat=true;
    else onHeat=false;
    qDebug()<<"Calculated T0 ="<<T0<<"T1 ="<<T1<<"dT ="<<dT;


    qDebug()<<"Calculating F parameters";
    double dF,Favg,tau1=0,tau2=0;
    //determining dF and Favg
    switch (FapproximationMethod) {
    case NonLinearApproximation::methodLine:
        dF=paramsF.at(1)-(FCurve->sample(plot->getT0Index()).y());
        Favg=(FCurve->sample(plot->getT0Index()).y())+dF/2;
        break;
    case NonLinearApproximation::methodExp:
        //b - shows limit of exponential growth and dF at x=0 for line
        dF=paramsF.at(0)-(FCurve->sample(plot->getT0Index()).y());
        Favg=(FCurve->sample(plot->getT0Index()).y())+dF/2;
        if (FapproximationMethod==NonLinearApproximation::methodExp) {
            tau1=paramsF.at(2);
        }
        break;
    case NonLinearApproximation::methodExpLine:
        dF=paramsF.at(0)-(FCurve->sample(plot->getT0Index()).y());
        Favg=FCurve->sample(plot->getT0Index()).y()+dF/2;
        tau1=paramsF.at(2);
        break;
    case NonLinearApproximation::methodExpExpLine:
        dF=paramsF.at(0)+paramsF.at(1)-(FCurve->sample(plot->getT0Index()).y());
        Favg=FCurve->sample(plot->getT0Index()).y()+dF/2;
        tau1=paramsF.at(2);
        tau2=paramsF.at(3);
        qWarning()<<"FIXME: Probably fixes needed";
        break;
    default:
        qWarning()<<"FIXME: Not implelemted";
        break;
    }
    qDebug()<<"Calculated dF ="<<dF<<"Favg ="<<Favg<<"tau1 ="<<tau1;
    qDebug()<<"setting thermal expansion parameters to dilatometerData";
    dilatometerData->setThermalExpansion(T0,T1,dF,Favg,tau1,tau2);

    QVector<QPointF> dTCurveData;
    QVector<QPointF> dFCurveData;

    dTCurveData.append(QPointF(TCurve->sample(plot->getT0Index())));
    dTCurveData.append(QPointF(plot->getT0(),plot->getT0Value(TCurve)+dT));

    dFCurveData.append(QPointF(FCurve->sample(plot->getT0Index())));
    dFCurveData.append(QPointF(plot->getT0(),plot->getT0Value(FCurve)+dF));


    plot->addTemporaryCurve(dTCurveData,TCurve);
    plot->addTemporaryCurve(dFCurveData,FCurve);
}


void MainWindow::showConfigurationDialog() {
    BasicConfigurationDialog *dialog=new BasicConfigurationDialog(this);
    dialog->exec();
    delete dialog;
}

void MainWindow::showCalculationLog() {
    if (dilatometerData->getLog().isEmpty()) {
        QMessageBox::information(this,tr("No log available"),"No calculation log available");

    } else {
        QMessageBox::information(this,tr("Last calculation"),dilatometerData->getLog());
    }
}
