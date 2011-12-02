#ifndef QABSTRACTDEVICE_H
#define QABSTRACTDEVICE_H

#include <QObject>
#include <QByteArray>

/// Bus type enumertion. Gpib bus or some other.
enum deviceBusType {
    Gpib,
    Multi
};

class QAbstractDevice : public QObject
{
    Q_OBJECT
public:
    //
    explicit QAbstractDevice(enum deviceBusType busType=Gpib, QObject *parent = 0);
    ~QAbstractDevice();


    /** The MAIN function of the deivce :)
      */
    virtual bool readValue(int channel, QByteArray *returnValue)=0;

    /// Amount of received data from physical device/driver
    virtual long int rx(void)=0;

    /// Amount of data transmitted to physical device/driver
    virtual long int tx(void)=0;

    /// Number of all devices (instances of this class)
    static int deviceCount(void);

    /// Device number among all the devices
    int deviceNum(void) const;

    /// Bus type of the device. GPIB or some other hw bus
    deviceBusType busType(void) const;

private:
    /// Number of the instances of this class
    static int numdevices;

    /** Number of the current instance.
      Each device then can know that it is n'th of total m devices
    */
    int numdevice;

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



signals:
    void rx(int);
    void tx(int);

public slots:

};

#endif // QABSTRACTDEVICE_H
