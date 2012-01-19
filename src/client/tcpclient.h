#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTcpSocket>
#include <QBuffer>

class TcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = 0);
    enum Expect {
        expectCommand,
        expectData,
        expectInterval
    };

private:
    QByteArray buffer;
    Expect expect;
    void setExpect(Expect Expectation);
    Expect getExpect(void) const;


signals:

    /// Signal is emitted when we have new line with comment or data to be passed to ExperimentData
    void dataLine(QByteArray &dataLine);
//    /// Signal is emitted when new portion of data is arrived to buffer;
//    void newDataArrived(void);

    /// Signal is emitted when initial data is come. It should be connected to ExperimentData to clear the data
    void initialData(void);
    /// Signal is emitted when server stops to measure
    void serverStatus (bool running);
    /// Signal is emitted when server changes measuring interval
    void serverInterval(int interval);

public slots:

    void askInitialData();
    void getData();
    /** Slot gets the line from network and process it.
      It can emit dataLine signal to ExperimentData if it's comment or data line.
      Or it can emit other signals to update app information about the server (i.e. interval change or status change)
      */
    void protocolParser(QByteArray& line);

};

#endif // TCPCLIENT_H
