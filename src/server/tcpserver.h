#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QDebug>
#include <QTcpSocket>
#include <QTcpServer>
#include "experiment.h"

class TcpServer : public QTcpServer
{
Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
    int getClientCount(void) const;
    void setExperiment(Experiment *experiment);
    void setClientMonitoringMode(QTcpSocket * socket, bool on);
    bool getClientMonitoringMode(QTcpSocket * socket) const;

private:
    QSet<QTcpSocket *> clientSocket;
    QHash<QTcpSocket *, bool> clientSocketMonitorMode;

    Experiment *experiment;

private slots:
    void acceptConnection();
    void removeConnection();
    void readCommand(void);
    void measured(QString dataLine);


signals:
    void clientCountChanged(int count);

public slots:
    void experimentStatusChanged(bool running);
    void experimentIntervalChanged(int msec);
    void experimentForbidden(QString message);
    void disconnectClients(void);

};

#endif // TCPSERVER_H
