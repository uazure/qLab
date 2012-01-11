#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QDebug>
#include <QTcpSocket>
#include <QTcpServer>
#include "qexperiment.h"

class TcpServer : public QTcpServer
{
Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
    int getClientCount(void) const;
    void setExperiment(QExperiment *experiment);

private:
    QSet<QTcpSocket *> clientSocket;
    QExperiment *experiment;

private slots:
    void acceptConnection();
    void removeConnection();
    void readCommand(void);


signals:
    void clientCountChanged(int count);

public slots:
    void experimentStatusChanged(bool running);
    void disconnectClients(void);

};

#endif // TCPSERVER_H
