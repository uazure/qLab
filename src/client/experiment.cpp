#include "experiment.h"

Experiment::Experiment(TcpClient* tcp, QObject *parent) :
    QObject(parent)
{
    interval=0;
    tcpClient=tcp;
    connect(tcpClient,SIGNAL(connected()),SLOT(initialize()));
}

void Experiment::initialize() {
    tcpClient->query(TcpClient::queryInitialData);
    tcpClient->query(TcpClient::queryControls);
    tcpClient->query(TcpClient::queryInterval);
    tcpClient->query(TcpClient::queryStatus);
}

QStringList Experiment::getControl() const {
    return control;
}

void Experiment::setControl(QStringList controls) {
    control=controls;
}

int Experiment::getInterval() const {
    return interval;
}

void Experiment::setInterval(int newInterval) {
    interval=newInterval;
}
