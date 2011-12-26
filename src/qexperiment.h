#ifndef QEXPERIMENT_H
#define QEXPERIMENT_H

#include <QObject>
#include "qabstractdevice.h"
#include <QString>
#include <QList>
#include <QStringList>
#include <QSettings>
#include <QApplication>
#include "devicefarm.h"

class QExperiment : public QObject
{
Q_OBJECT

public:
    explicit QExperiment(QString name="",QObject *parent = 0);
    ~QExperiment();

    void initDevices();


    private:
    QString name;
    QStringList deviceStringList;
    QList <QAbstractDevice *> deviceList;
    QList <QStringList> parametersList;
    QSettings *settings;


signals:

public slots:
    void setExperiment(QString experiment);
};

#endif // QEXPERIMENT_H
