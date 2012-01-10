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
    void incrementClientCount(void);
    void decrementClientCount(void);
    int getClientCount(void) const;
    void setExperiment(QExperiment *experiment);

private:
    //QList<QTcpSocket *> clientSocket;
    int clientCount;
    QExperiment *experiment;

private slots:
    void acceptConnection();
    void removeConnection();
    void readCommand(void);

signals:
    void clientCountChanged(int count);

public slots:

};

#endif // TCPSERVER_H
