#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <QObject>
#include <QTimer>
#include "abstractdevice.h"
#include <QString>
#include <QList>
#include <QStringList>
#include <QSettings>
#include <QApplication>
#include <QFile>
#include "devicefarm.h"
#include "controllabledevicelist.h"
#include "experimenthistory.h"

// minimal interval is 100 msec
#define EXPERIMENT_MIN_INTERVAL 100
#define EXPERIMENT_MAX_INTERVAL 60000

class Experiment : public QObject,
        public ControllableDeviceList,
        public ExperimentHistory
{
Q_OBJECT

public:
    explicit Experiment(QString name="",QObject *parent = 0);
    ~Experiment();

    void initDevices();

    void initControllers();

    bool isActive(void) const;
    void setFileName(QString filename);
    QString getCurrentFileName() const;
    int getInterval() const;
    /*Get the header of the experiment. Header is the text data
    (commented out) with description of the experiment settings and
    measured units, limits, etc.
    */
    QString getHeader(void) const;
    QString getData(void) const;
    QStringList getDataStringList(void) const;
    QString getName(void) const;
    static const int min_interval=EXPERIMENT_MIN_INTERVAL;
    static const int max_interval=EXPERIMENT_MAX_INTERVAL;

    private:

    QString name;
    void setName(QString name);
    int timer_progress;

    QStringList deviceStringList;
    QStringList axisStringList;
    QList <AbstractDevice *> deviceList;
    QList <QStringList> parametersList;
    QSettings *settings;
    QTimer timer;
    QTimer progressTimer;
    QTimer saveTimer;
    QDateTime startedOn;
    QStringList dataStringList;
    QString currentFileName;
    AbstractDevice * findDevice (QString deviceName) const;

private slots:
    void updateProgress(void);


signals:
    /// When experiment name (configuration) has changed
    void experimentChanged(QString experimentName);
    /// When measure cycle performed
    void measured (QString line);
    /// When status (running/idle) changed
    void statusChanged (bool isOnline);
    /// When measuring interval changed
    void intervalChanged(int msec);
    /// When active file has changed
    void fileChanged(QString filename);
    /// To notify user about something
    void Notify(QString message);
    /// To notify tcp clients that they are forbidden to change settings
    void TcpForbidden(QString message);
    /// says to update progress-meter for interval. progress is from 0 to 100;
    void updateProgress(int progress);
    /// informs that target value has changed
    void targetChanged(QString target);

public slots:
    /// set experiment name (configuration)
    void setExperiment(QString experiment);
    /// force measuring cycle
    void doMeasure(void);
    /// start timer. To start measuring immediately consider call doMeasure() before start()
    void start(bool);
    void start(void);
    /// stop timer
    void stop(void);
    /// set the measuring interval
    void setInterval(int msec);

    /// set target value for specific control loop
    void setTarget(QString value,int control=0);

    /// get target value for specific control loop
    QString getTarget(int control=0);

    /// Saves data to active file
    void saveFile();

};

#endif // EXPERIMENT_H
