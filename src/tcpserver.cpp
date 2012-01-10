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
    experiment=0;
}

void TcpServer::acceptConnection() {
    while (hasPendingConnections()) {
        QTcpSocket *socket=nextPendingConnection();
        incrementClientCount();
        qDebug()<<"Client connected. Client count:"<<getClientCount();
        connect(socket,SIGNAL(disconnected()),this,SLOT(removeConnection()));
        connect(socket,SIGNAL(readyRead()),this,SLOT(readCommand()));
        if (!experiment) {
            qWarning()<<"No pointer to experiment object set in TcpServer class";
            continue;
        }

    }
}

void TcpServer::removeConnection() {
    decrementClientCount();
    qDebug()<<"Client disconnected. Client count:"<<getClientCount();
    //FIXME:
    //Here we need to know ip and port of QTcpSocket to remove it from the list.
    //we can make a dirty hack to get this info from sender() pointer, but there must be a better way...
}

void TcpServer::incrementClientCount(void) {
    ++clientCount;
    emit clientCountChanged(clientCount);
}

void TcpServer::decrementClientCount(void) {
    --clientCount;
    emit clientCountChanged(clientCount);
}

int TcpServer::getClientCount() const {
    return clientCount;
}

void TcpServer::setExperiment(QExperiment *experiment) {
    this->experiment=experiment;
}

void TcpServer::readCommand() {

}
