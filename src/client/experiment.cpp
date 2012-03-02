#include "experiment.h"

Experiment::Experiment(TcpClient* tcp, QObject *parent) :
    QObject(parent)
{
    interval=0;
    tcpClient=tcp;
    connect(tcpClient,SIGNAL(connected()),SLOT(initialize()));
    connect(tcpClient,SIGNAL(serverControlTarget(int,QString)),SLOT(setControlTarget(int,QString)));
    connect(tcpClient,SIGNAL(serverControlList(QStringList)),SLOT(setControl(QStringList)));
}

void Experiment::initialize() {
    tcpClient->query(TcpClient::queryInitialData);
    tcpClient->query(TcpClient::queryControls);
    tcpClient->query(TcpClient::queryInterval);
    tcpClient->query(TcpClient::queryStatus);
}

QStringList Experiment::getControl() const {
    return controlList;
}

void Experiment::setControl(QStringList controls) {
    controlList=controls;
}

int Experiment::getInterval() const {
    return interval;
}

void Experiment::setInterval(int newInterval) {
    interval=newInterval;
}

QString Experiment::getControlTarget(int controlIndex) const {
    if (controlIndex>=targetList.size()) {
        qWarning()<<"Control index larger that targets array";
        return "Error";
    }
    return targetList.at(controlIndex);
}

void Experiment::setControlTarget(int controlIndex, QString target) {
    if (targetList.size()<=controlIndex) {
        qWarning()<<"Control index larger than targets array. Appending";
        while (targetList.size()<=controlIndex) {
            targetList.append(QString());
        }
    }
    targetList[controlIndex]=target;
}

QStringList Experiment::getHistory() const {
    if (historyList.isEmpty()) {
        tcpClient->query(TcpClient::queryHistory);
    }
    return historyList;
}

void Experiment::setHistory(QStringList &history) {
    qDebug()<<"History updated\n"<<history;
    historyList=history;
}
