#include "udpserver.h"

UdpServer::UdpServer(QObject *parent) :
    QUdpSocket(parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(readCommand()));
    //bind to port 25050
    if (!bind(25050)) {
        qWarning()<<"Failed to bind to port 25050";
    } else {
        qDebug()<<"Bound to port 25050";
    }
    experiment=0;
}


void UdpServer::readCommand() {
    if (bytesAvailable()<0) {
        qWarning()<<"No data to read from udp socket";
        return;
    }
    if (!experiment) {
        qWarning()<<"No experiement set";
        return;
    }

    QByteArray buf;
    QByteArray reply;
    buf.resize(pendingDatagramSize());
    QHostAddress host;
    quint16 port;

    if (readDatagram(buf.data(),buf.size(),&host,&port)<0) {
        qWarning()<<"Failed to read datagram";
        return;
    }
    qDebug()<<"buf:"<<buf;


    if (buf.startsWith("gpib scan")) {
        reply="200 You found me\n";
    } else if (buf.startsWith("get interval")) {
        reply="200 Interval:\n";
        reply.append(QByteArray::number(experiment->getInterval())).append("\n");
    } else if (buf.startsWith("ping")) {
        reply="pong\n";
    } else if (buf.startsWith("get all")) {
        reply="200 Initial data:\n";
        reply+=this->prepareData();
    } else if (buf.startsWith("status")) {
        if (experiment->isActive())
            reply="200 Running\n";
        else reply="200 Idle\n";
    }


    if (reply.size()>0) {
        qDebug()<<"Reply:"<<reply;
        this->writeDatagram(reply,host,port);
        return;
    } else {
        qWarning()<<"No reply to"<<host<<":"<<port;
        return;
    }

}

void UdpServer::setExperiment(Experiment *experiment) {
    this->experiment=experiment;
    data=experiment->getDataStringList();
}

QByteArray UdpServer::prepareData(int index) {
    //FIXME: This either needs fixing or we doesn't need that at all! It doesn't work with qlabnetview :(
    if (index<0) {
        qDebug() << "prepareData: index is negative. aborting";
        return "";
    }
    int i=index;
    QByteArray answer;
    while (answer.size()<UDP_SIZE && i<data.size()) {
        answer.append(this->data.at(i).trimmed()).append("\n");
        qDebug()<<"appended "<<this->data.at(i).trimmed();
        i++;
        }

    if (answer.size()>=UDP_SIZE) {
        answer.truncate(answer.size()-data.at(i-1).trimmed().size()-2);
        }

    return answer;

}
