#ifndef QABSTRACTDEVICE_H
#define QABSTRACTDEVICE_H

#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <QStringList>
//#include "experiment.h"
class Experiment;

/// Bus type enumertion. Gpib bus or some other.
enum deviceBusType {
    NoBus,
    Time,
    Utime,
    Gpib,
    Multi
};

class AbstractDevice : public QObject
{
    Q_OBJECT
public:
    //
    explicit AbstractDevice(Experiment *exp = 0);
    ~AbstractDevice();


    /** The MAIN function of the deivce :)
      */
    virtual bool readValue(QByteArray &returnValue, QStringList parameters=QStringList())=0;

    /// Second main function :)
    virtual void resetDevice(void);

    /** Function that initializes device before use. Default is to do nothing.
      This function normally should be overwritten by child classes if
      devices do require some initialization. */


    /// Amount of received data from physical device/driver
    //virtual long int rx(void)=0;

    /// Amount of data transmitted to physical device/driver
    //virtual long int tx(void)=0;

    /// Number of all devices (instances of this class)
    static int deviceCount(void);

    /// Device number among all the devices
    int deviceNum(void) const;

    /// Bus type of the device. GPIB or some other hw bus
    deviceBusType busType(void) const;

    /// Shortcut for the device. It's used in .ini file to set up device properties
    //const QString shortName(void) const;
    const QByteArray shortName(void) const;

    double getFactor(void) const;
    void setFactor(double factor);

    double getMinValue(void) const;
    void setMinValue(double min_value);

    double getMaxValue(void) const;
    void setMaxValue(double max_value);

    double getScaleHint(void) const;
    void setScaleHint(double scale_hint);

    const QString getUnit(void) const;
    void setUnit(QString unit);

    const QString getLabel(void) const;
    void setLabel(QString label);

    bool capable(QString capability) const;
    void setCapability(QString capability);

    QByteArray getLastValue(void) const;


private:
    /// Number of the instances of this class
    static int numdevices;

    /** Number of the current instance.
      Each device then can know that it is n'th of total m devices
    */
    int numdevice;

    /** Factor - is the coefficient that is used to multiply value from
      the measurement device to get value in the real-work units.
      Units are specified by unit variable;
      sa: unit
      */
    double factor;

    /** min_value is the minimum adequate value that can be obtained
      from the device. For example, temperature can not be lower that 0 K
      or higher than 300 K if thermometer is graduated up to 300 K.
      sa: max_value, scale_hint, unit
      */
    double min_value;

    /** max_value is the maximum adequate value that can be obtained
    from the device. min_value and max_value may be used for example
    by crossover function
    sa: min_value, factor, unit
      */
    double max_value;

    /** scale_hint is the value in units that is minimum scale for
    adequate reproduction of the data. Maximum scale can be determined
    from min_value and max_value
    sa: max_value, min_value,factor
      */
    double scale_hint;

    /** unit is the string representing the unit name for collected data.
      Example of units: K (for Kelvin degrees), Hz (for frequency),
    Torr (for pressure measurements). Data from measuring device must be
    multiplied factor times to be become values in units.
    sa: factor
      */
    QString unit;

    /** label is the string with description of what is being measured
      by this device. For example - "temperature of the sample"
      sa: unit
      */
    QString label;


protected:
    /// Device bus type
    int bus;

    /** Device short name. It's used to refer to device in config files.
      Ex. k2700, cc34
      */
    QByteArray shortname;

    /** Device identification string (if available).
      Ex. Device identification can be obtained by 'IDN?' command for GPIB interface
      */
    QByteArray idn;

    QStringList capabilities;

    QByteArray lastValue;

    Experiment * experiment;



signals:
    void rx(int);
    void tx(int);
    void errorMessage (QString);

public slots:

};

#endif // QABSTRACTDEVICE_H
