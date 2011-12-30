#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QDebug>
#include <QTcpSocket>
#include <QTcpServer>

class TcpServer : public QTcpServer
{
Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
    void setClientCount(int);
    int getClientCount(void) const;

private:
    QList<QTcpSocket *> clientSocket;
    int clientCount;

private slots:
    void acceptConnection();
    void removeConnection();

signals:
    void clientCountChanged(int count);

public slots:

};

#endif // TCPSERVER_H
