#ifndef ABSTRACTTHERMOCONTROLLERDEVICE_H
#define ABSTRACTTHERMOCONTROLLERDEVICE_H

#include <QString>
#include <QVector>
#include <QDebug>

class AbstractThermocontrollerDevice
{
// Q_OBJECT

public:
    explicit AbstractThermocontrollerDevice();

    /** \brief Set target temperature of control loop
      \sa setControlChannel(), getTargetValue()
      */
    virtual bool setTargetValue(QString value,int controlLoop=0)=0;

    QString getTargetValue(int controlLoop=0) const;

    /** \brief Set control channel for coltrol loop.

      Usually thermocontrollers can measure temperature on several
      channels. Each channel is an input and can be read with
      readValue() function that is re-implemented in gpibdevice
      Thermocontrollers are able to control one or more input channels.
      This function is used to set controlled channel for the control
      loop.
      \sa getControlChannel()
      */

    virtual bool setControlChannel(QString channel,int controlLoop=0)=0;

    virtual QString getControlChannel(int controlLoop=0)=0;

    /** \brief Reports if control loop is engaged.

      Thermocontroller can act just like a measuring device without
      controlling the channel. This function reports if control is
      currently engaged.
      */
    bool isControlEngaged(int controlLoop=0) const;
    /** \brief Engage control loop for controlling the temperature

      */
    virtual void engageControl(bool engage,int controlLoop=0);


    // PID access functions
    double getP(int controlLoop=0) const;
    double getI(int controlLoop=0) const;
    double getD(int controlLoop=0) const;

    //set functions for PID are virtual. Some actions should be performed on changing PID
    virtual void setP(double p,int controlLoop=0);
    virtual void setI(double i,int controlLoop=0);
    virtual void setD(double d,int controlLoop=0);

    void setControlLoopNum(int loops);
    int getControlLoopNum(void) const;

    QString getLoopName(int num) const;
    int getLoopIndex(QString name) const;


protected:
    int controlLoopNum;
    QVector<bool> controlEngaged;
    //PID coefficients
    QVector <double> P,I,D;

    QVector<QString> controlChannel;
    QVector<QString> targetValue;
    QVector<QString> loopName;
    QVector<QString> channelName;

};

#endif // ABSTRACTTHERMOCONTROLLERDEVICE_H
