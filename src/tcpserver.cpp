#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) :
    QTcpServer(parent)
{
    quint16 port=25050;
    while (!listen(QHostAddress::Any,port) && port<65535) {
        qWarning()<<"Failed to listen port"<<port;
        ++port;
    }
    qDebug()<<"Listening on port"<<serverPort();
    connect(this,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
    clientCount=0;
}

void TcpServer::acceptConnection() {
    while (hasPendingConnections()) {
        clientSocket.append(nextPendingConnection());
        setClientCount(getClientCount()+1);
        QTcpSocket *socket=clientSocket.last();
        connect(socket,SIGNAL(disconnected()),this,SLOT(removeConnection()));
    }
}

void TcpServer::removeConnection() {

    qDebug()<<"Remove connection";
    setClientCount(getClientCount()-1);
    //FIXME:
    //Here we need to know ip and port of QTcpSocket to remove it from the list.
    //we can make a dirty hack to get this info from sender() pointer, but there must be a better way...
}

void TcpServer::setClientCount(int count) {
    clientCount=count;
    emit clientCountChanged(count);
}

int TcpServer::getClientCount() const {
    return clientCount;
}
