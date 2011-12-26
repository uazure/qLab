#ifndef QEXPERIMENT_H
#define QEXPERIMENT_H

#include <QObject>
#include "qabstractdevice.h"
#include <QString>
#include <QList>
#include <QStringList>

class qExperiment : public QObject
{
Q_OBJECT

public:
    explicit qExperiment(QString name,QObject *parent = 0);


    private:
    QString name;
    QList <QAbstractDevice *> deviceList;
    QList <QStringList> parameters;
    void initDevices();

signals:

public slots:

};

#endif // QEXPERIMENT_H
