#ifndef UTIMEDEVICE_H
#define UTIMEDEVICE_H

//#include "experiment.h"
#include "abstractdevice.h"
#include <QTime>
#include <QDateTime>
#include <QByteArray>
#include <QString>

/**  This class provides device that returns current time as
unix timestamp with milliseconds resolution
  */
class UtimeDevice : public AbstractDevice
{
Q_OBJECT
public:
    explicit UtimeDevice(Experiment *exp = 0);
    /// Implementation of pure virtual method of QAbstractDevice
    bool readValue(QByteArray &returnValue, QStringList parameters=QStringList());

    /** static function that returns current time with
      milisecods precision
      */
    inline static QString readValue(void) {
        return QString::number(QDateTime::currentDateTime().toTime_t())+"."+QTime::currentTime().toString("zzz");
    };

signals:

public slots:

};

#endif // UTIMEDEVICE_H
