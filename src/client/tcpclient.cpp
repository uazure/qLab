#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent) :
    QTcpSocket(parent)
{
    connect(this,SIGNAL(connected()),this,SLOT(askInitialData()));
    connect(this,SIGNAL(readyRead()),this,SLOT(getData()));
    expect=expectCommand;
}

void TcpClient::askInitialData()
{
    this->write("get all\n");
}

void TcpClient::getData()
{
    qint64 bytes=bytesAvailable();
    QByteArray buf;
    if (bytes<1) {
        qWarning()<<"No bytes available to read from network";
        //return;
    }
    while (canReadLine()) {
        buf=readLine(bytes);
        protocolParser(buf);
    }

    //    buf=readAll();
    //    qDebug()<<"Read buffer: "<<buf;
    //    if (buf.startsWith("400") || buf.startsWith("403") || buf.startsWith("404")) {
    //        qWarning()<<"Server said:"<<buf;
    //        return;
    //    }
    //    if (buf.startsWith("200 Initial data:")) {
    ////truncate first string (evertything before \n)
    //        buf=buf.right(buf.size()-buf.indexOf('\n')-1);
    //        emit initialData(buf);
    //}


}

void TcpClient::protocolParser(QByteArray &line) {
    if (line.size()<1) {
        qWarning()<<"protocolParser: Line size is"<<line.size();
        qDebug()<<"protocolParser:"<<line;
    }
    line=line.trimmed();

    //If line is empty after trimming it means it was just empty!
    //And empty lines prepends the commands from server :)
    if (line.isEmpty()) {
        setExpect(expectCommand);
        return;
    }


    if (getExpect()==expectData && !line.startsWith("200 ")) {
        emit dataLine(line);
        return;
    }

    if (getExpect()==expectInterval) {
        int a;
        bool ok=false;
        a=line.toInt(&ok);
        if (ok) {
            emit serverInterval(a);
        } else {
            qWarning()<<"Failed to get interval integer from string"<<line;
        }
        return;
    }

    if (line.startsWith("200 Initial data:")) {
        emit initialData();
        setExpect(expectData);
    } else if (line.startsWith("200 Data:")) {
        setExpect(expectData);
    } else if (line.startsWith("200 Interval:")) {
        setExpect(expectInterval);
    } else if (line.startsWith("200 Idle")) {
        emit serverStatus(false);
        setExpect(expectCommand);
    } else if (line.startsWith("200 Running")) {
        emit serverStatus(true);
        setExpect(expectCommand);
    }
}

void TcpClient::setExpect(Expect expectation) {
    expect=expectation;
}

TcpClient::Expect TcpClient::getExpect() const {
    return expect;
}
