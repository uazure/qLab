#include "experiment.h"

Experiment::Experiment(TcpClient* tcp, QObject *parent) :
    QObject(parent)
{
    interval=0;
    tcpClient=tcp;
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
