#ifndef QEXPERIMENT_H
#define QEXPERIMENT_H

#include <QObject>
#include <QTimer>
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
    void setName(QString name);
    QString getName(void) const;
    QStringList deviceStringList;
    QList <QAbstractDevice *> deviceList;
    QList <QStringList> parametersList;
    QSettings *settings;
    QTimer *timer;

private slots:



signals:
    void experimentChanged(QString experimentName);
    void measured (QString line);

public slots:
    void setExperiment(QString experiment);
    void doMeasure(void);
};

#endif // QEXPERIMENT_H
