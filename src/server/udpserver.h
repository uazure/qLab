#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QObject>
#include <QUdpSocket>
#define UDP_SIZE 1300

#include "experiment.h"

class UdpServer : public QUdpSocket
{
Q_OBJECT
public:
    explicit UdpServer(QObject *parent = 0);
    void setExperiment(Experiment *experiment);

signals:

public slots:

    void readCommand();

private:
    Experiment *experiment;
    QByteArray prepareData(int index=0);
    QStringList data;

};

#endif // UDPSERVER_H
