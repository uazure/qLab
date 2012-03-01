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
    experiment=0;
}

void TcpServer::acceptConnection() {
    while (hasPendingConnections()) {
        QTcpSocket *socket=nextPendingConnection();
        //remember connection
        clientSocket.insert(socket);
        //set monitoring mode off (it will be swithced to on when client ask for initial data
        setClientMonitoringMode(socket,false);

        //inform that we have +1 to connection count
        emit clientCountChanged(clientSocket.count());

        qDebug()<<"Client connected. Client count:"<<getClientCount();
        connect(socket,SIGNAL(disconnected()),this,SLOT(removeConnection()));
        connect(socket,SIGNAL(readyRead()),this,SLOT(readCommand()));
        socket->write("You are connected to qLab server\n");

        if (!experiment) {
            qWarning()<<"No pointer to experiment object set in TcpServer class";
            continue;
        }

    }
}

void TcpServer::removeConnection() {
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket) return;

    if (clientSocket.remove(socket)) {
        clientSocketMonitorMode.remove(socket);
        emit clientCountChanged(clientSocket.count());
    }
    else {
        qWarning()<<"";
    }
    qDebug()<<"Client disconnected. Client count:"<<getClientCount();
    socket->deleteLater();
}



int TcpServer::getClientCount() const {
    return clientSocket.count();
}

void TcpServer::setExperiment(Experiment *experiment) {
    this->experiment=experiment;
    //measured data from experiment will go to all clients that are in monitoring mode
    connect(experiment,SIGNAL(measured(QString)),SLOT(measured(QString)));
}

void TcpServer::readCommand() {
    //Notice: I think of pointer type conversion as of dirty hack.
    //This was took from bittorrent qt example.
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    QByteArray peer;
    qint64 bufsize;

    //set peer as string containging "ip:port"
    peer.append(socket->peerAddress().toString().toUtf8()).append(':').append(QByteArray::number(socket->peerPort()));

    //if no bytes available in the socket then quit
    if (socket->bytesAvailable()<1) {
        qWarning()<<"bytesAvailable() ="<<socket->bytesAvailable()<<
                "peer"<<peer;
        return;
    }
    bufsize=socket->bytesAvailable();
    QByteArray buf;
    qDebug()<<"Bytes available to read from client"<<peer<<bufsize;
    if (!socket->canReadLine()) {
        qWarning()<<"Could not read line from client";
        qDebug()<<socket->readAll();
        return;
    }

    while (socket->canReadLine()) {
        buf=socket->readLine();
        protocolParser(buf,socket);
    }



}

void TcpServer::experimentStatusChanged(bool running) {
    QByteArray message;
    if (running) {
        message="\n200 Running\n";
    } else {
        message="\n200 Idle\n";
    }
    foreach (QTcpSocket *socket, clientSocket) {
        socket->write(message);
    }
}

void TcpServer::experimentIntervalChanged(int msec) {
    if (msec<100) {
        qWarning()<<"Interval is too low!"<<msec;
        return;
    }
    foreach (QTcpSocket *socket, clientSocket) {
        socket->write("\n200 Interval:\n"+QByteArray::number(msec)+'\n');
    }
}

void TcpServer::experimentForbidden(QString message) {
    foreach (QTcpSocket *socket, clientSocket) {
        socket->write("\n403 "+message.toUtf8()+'\n');
    }
}

void TcpServer::disconnectClients() {
    foreach (QTcpSocket *socket, clientSocket) {
        socket->write("\n200 You are about to be disconnected. Bye!\n");
        socket->disconnectFromHost();
    }
}

void TcpServer::setClientMonitoringMode(QTcpSocket *socket, bool on) {
    if (!socket) {
        qWarning()<<"Empty socket to set monitoring mode on";
        return;
    }
    clientSocketMonitorMode.insert(socket,on);
}

bool TcpServer::getClientMonitoringMode(QTcpSocket *socket) const {
    if (!socket) {
        qWarning()<<"Empty socket to get monitoring mode from";
        return false;
    }
    return clientSocketMonitorMode.value(socket);
}

void TcpServer::measured(QString dataLine) {
    QHashIterator<QTcpSocket *, bool> i(clientSocketMonitorMode);
    while (i.hasNext()) {
        i.next();
        if (i.value()) {
            i.key()->write("\n200 Data:\n"+dataLine.toUtf8().append("\n"));
        }
    }
}

void TcpServer::protocolParser(QByteArray &buf, QTcpSocket *socket) {
    //here we gonna recognize request and send an answer
    buf=buf.trimmed();
    if (buf=="ping") {
        socket->write("\n200 pong\n");
        return;
    }
    if (buf=="help") {
        socket->write("\n200 Help:\n"
                      "quit - close this channel\n"
                      "exit - close this channel\n"
                      "ping - ask if server is online. 'pong' reply should be get\n"
                      "version - program version\n"
                      "get interval - measuring interval in msec\n"
                      "set interval=NUMBER - set measuring interval in msec\n"
                      "get all - get initial experimental data with header\n"
                      "monitor on - set monitoring mode on\n"
                      "monitor off - set monitoring mode off\n"
                      "set target CONTROL=NUMBER - set target for specific CONTROL\n"
                      "set target=NUMBER - set target for default CONTROL\n"
                      "get target CONTROL - get target of specified control id\n"
                      "");
        return;
    }

    if (buf=="get interval") {
        socket->write("\n200 Interval:\n"+QByteArray::number(experiment->getInterval())+"\n\n");
        return;
    }
    if (buf.startsWith("set interval=")) {
        bool ok=false;
        int interval=buf.remove(0,13).trimmed().toInt(&ok);
        if (ok) {
            experiment->setInterval(interval);
        } else {
            qWarning()<<"Failed to get interval from request:\n"<<buf;
            socket->write("\n400 Bad request\nInterval should be specified as a number\n\n");
        }
        return;
    }

    if (buf.startsWith("set target=")) {
        bool ok=false;
        buf.remove(0,11).trimmed().toDouble(&ok);
        if (ok) {
            experiment->setTarget(buf.remove(0,11).trimmed());
            socket->write("\n200 Target for control 0 set\n");
            return;
        } else {
            qWarning()<<"Failed to recognize target value (NaN) of"<<buf;
            socket->write("\n400 Bad request\nTarget should be specified as a number\n\n");
            return;
        }
        return;
    }

    if (buf.startsWith("set target ")) {
        QList<QByteArray> tmp=buf.remove(0,11).split('=');
        if (tmp.size()!=2) {
            qWarning()<<"Failed to parse"<<buf;
            return;
        }
        bool ok=false;
        int controlIndex=tmp.at(0).trimmed().toInt(&ok);
        qDebug()<<tmp;
        if (!ok) {
            qWarning()<<"Failed to parse"<<buf<<": control index NaN";
            socket->write("\n400 Bad request\nCONTROL index should be integer");
            return;
        }
        if (controlIndex>=experiment->ControllableDeviceList::size() || controlIndex<0) {
            qWarning()<<"Control index is not correct"<<controlIndex;
            socket->write("\n400 Bad request\nControl index incorrect");
            return;
        }

        double value=tmp.at(1).trimmed().toDouble(&ok);
        if (!ok) {
            qWarning()<<"Failed to parse"<<buf<<": target value NaN";
            socket->write("\n400 Bad request\nTarget value should be double");
            return;
        }
        experiment->setTarget(tmp.at(1).trimmed(),controlIndex);
        QByteArray reply="\n200 Target of control ";
        reply+=controlIndex;
        reply+="set";

        socket->write(reply);
        return;

    }

    if (buf.startsWith("get target")) {
        buf=buf.remove(0,10);
        buf=buf.trimmed();
        bool ok;
        int index;
        index=buf.toInt(&ok);
        if (!ok) {
            index=0;
        }
        if (index>=experiment->size()) {
            socket->write("400 Bad request\nTarget with such index does not exists");
            qWarning("Target index larger than number of controls");
            return;
        }
        QString target="200 Target of control:\n";
        target.append(QString::number(index)).append("\t");
        target.append(experiment->getTarget(index).append("\n\n"));
        socket->write(target.toAscii());
        return;
    }

    if (buf=="get latest") {
        //socket->write(experiment->dataStringList)
    }

    if (buf=="get controls") {
        socket->write("\n200 Controls:\n");
        socket->write(experiment->getControlList().join("\n").toAscii().append("\n\n"));
        return;
    }

    if (buf=="get all") {
        socket->write("\n200 Initial data:\n");
        socket->write(experiment->getHeader().toUtf8 ());
        socket->write(experiment->getData().toUtf8());
        socket->write("\n\n");
        setClientMonitoringMode(socket, true);
        return;
    }

    if (buf=="monitor on") {
        setClientMonitoringMode(socket,true);
        socket->write("\n200 Monitoring on\n\n");
        return;
    }

    if (buf=="monitor off") {
        setClientMonitoringMode(socket,false);
        socket->write("\n200 Monitoring off\n\n");
        return;
    }

    if (buf=="get monitor") {
        if (getClientMonitoringMode(socket)) {
            socket->write("\n200 Monitoring on\n\n");
        } else {
            socket->write("\n200 Monitoring off\n\n");
        }
        return;
    }

    if (buf=="start") {
        experiment->start();
        return;
    }

    if (buf=="stop") {
        experiment->stop();
        return;
    }

    if (buf=="status") {
        if (experiment->isActive())
           socket->write("\n200 Running\n\n");
        else
           socket->write("\n200 Idle\n\n");
        return;
    }


    if (buf=="version") {
        socket->write(QCoreApplication::applicationName().toUtf8()+" version "+QCoreApplication::applicationVersion().toUtf8()+"\n\n");
        return;
    }

    if (buf=="quit" || buf=="exit") {
        socket->write("\n200 Bye!\n\n");
        socket->disconnectFromHost();
        return;
    }


    //if we did not recogized request
        socket->write("\n\n400 Bad request\nType 'help' for full list of supported commands\n\n");

}
