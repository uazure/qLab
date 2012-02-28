#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <QObject>
#include <QStringList>
#include "tcpclient.h"

class Experiment : public QObject
{
Q_OBJECT
public:
    explicit Experiment(TcpClient *,QObject *parent = 0);

signals:

public slots:
    void setInterval(int interval);
    int getInterval() const;

    QStringList getControl() const;
    void setControl(QStringList controls);

private:
    int interval;
    QStringList control;
    TcpClient *tcpClient;

};

#endif // EXPERIMENT_H
