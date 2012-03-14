#include "gpibdevice.h"

GpibDevice::GpibDevice(QByteArray &DeviceShortName, Experiment *exp) :
        AbstractDevice (exp)

{
    bus=Gpib;
    // initialising gpib external variables;
    ibsta=0x11;
    iberr=0x22;
    ibcnt=0x33;
    ibcntl=0x44;

    shortname=DeviceShortName;
    deviceSettings=new QSettings(QSettings::IniFormat,QSettings::UserScope,
                                 QApplication::organizationName(),"devices");
    experimentSettings=new QSettings(QSettings::IniFormat,QSettings::UserScope,
                                     QApplication::organizationName(),"experiment");

    /// Read id from settings object (ini file)
    id=deviceSettings->value(shortname+"/id",0).toInt();
    if (!id) {
        qWarning()<<"Device:"<<shortname<<"id read from config file is"<<id;
    }

    // define handle to speak to device using ibwrt/ibrd functions
#ifdef WIN32
    handle=ibdev(0,id,NO_SAD,T300ms,1,0);

    if (ibsta&ERR)
    {
        qDebug()<<"Gpib error: ibdev failed on device"<<shortName()<<"with id"<<Id();
        emit errorMessage("Gpib error: ibdev failed");
    }

    if (isOnline()) {
        this->getIdn();
    } else {
        qWarning()<<"Device"<<shortName()<<"id"<< Id() <<"is OFFLINE";
    }

    //resetDevice();
#endif

    // Read init string for the device from settings object
    QString tmpstring=deviceSettings->value (shortname+"/command/init").toString();
    QStringList initStringList = tmpstring.split("\n",QString::SkipEmptyParts,Qt::CaseSensitive);
    qDebug()<<initStringList;

    //go thru init strings and execute each string as gpib command
    for (int i=0;i<initStringList.size();i++) {
        if (! set (initStringList.at(i).toLocal8Bit())) {
            qWarning()<<"Device"<<shortname<<"init with"<<initStringList.at(i)<<"failed";
        } else {
            qDebug()<<"Device"<<shortname<<"init with"<<initStringList.at(i)<<"OK";
        }
    }

    //next step is to identify read and channel commands
    readCommand=deviceSettings->value(shortname+"/command/read").toString();
    if (readCommand.isEmpty()) {
        qWarning()<<"Device"<<shortname<<"read command is empty";
    } else {
        qDebug()<<"Device"<<shortname<<"read command is"<<readCommand;
    }

    channelCommand=deviceSettings->value(shortname+"/command/channel").toString();
    if (!channelCommand.isEmpty()) {
        qDebug()<<"Device"<<shortname<<"channel command is"<<channelCommand;
    }


}

GpibDevice::~GpibDevice() {
    delete deviceSettings;
    delete experimentSettings;
}

/// Get identification string from the device
bool GpibDevice::getIdn() {
    QByteArray query("*IDN?");
    QByteArray response;
    if (ask (query,response)) {
        idn=response.trimmed();
        qDebug()<<"Gpib device"<<Id()<<"handle"<<Handle()<<"ident"<<idn;
        return true;
    } else {
        return false;
    }
}

/// Returns gpib id of the device.
int GpibDevice::Id() const {
    return id;
}

/// Returns gpib handle of the device
int GpibDevice::Handle() const {
    return handle;
}

/// Write data to the gpib device. Returns true on success or false on failure. Emits errorMessage on error.
bool GpibDevice::set(QByteArray command) {
#ifdef WIN32
    ibwrt(this->handle,command.data(),command.size()+1);
    if (ibsta&ERR) {
        qDebug()<<"Gpib error: ibwrt failed on device" <<shortName()<<"with id"<<Id();
        emit errorMessage("Gpib error: ibwrt failed");
        return false;
    }
#endif
    return true;
}

/// Read data from the gpib device. Returns true on success or false on failure.
bool GpibDevice::get(QByteArray &reply) {
#ifdef WIN32
    char *tmpchar=new char[255];
    //Clear array with 0. Some devices does not write \0 to the end of the string
    for (int i=0;i<255;i++) {
        tmpchar[i]=0;
    }
    ibrd(Handle(),tmpchar,254);
    if (ibsta&ERR) {
        qWarning()<<"Gpib error: ibrd failed on device"<<shortName()<<"with id"<<Id();
        emit errorMessage("Gpib error: ibrd failed");
        return false;
    }
    reply=tmpchar;
    reply=reply.trimmed();

#else
    reply=QByteArray::number((double) rand()/RAND_MAX*1000,'g',3);
#endif
    return true;
}

/// Handful method for asking gpib device for value. Returns true on success or false on failure.
bool GpibDevice::ask(QByteArray command, QByteArray &reply) {
    if (!set(command)) {
        return false;
    }

    if (!get(reply)) {
        return false;
    }
    return true;
}

/** \brief Implementation of AbstractDevice virtual primary method for gpib deivce

  We try to read value from the device in this function. We must have readCommand string.
  This can be specified in devices.ini file as command\read key in the device section.
  Example for k2000 device:
[k2000]
command\read=DATA?

  readCommand can also contain a special symbols like %1, %2 ... that can be substituted with the
  value that are specified as a parameter to the device
  Example for cc34 device:
[cc34]
command\read=INPUT? %1

  If parameterList is not empty then %1 will be replaced with first item in paramenterList in readCommand
  Example for experiment setup:
[experiment-debug]
out="utime,cc34(A)"

  There's argument "A" specified in brackets for cc34 device. This would result in reading data from the cc34
  with command:
INPUT? A

  Some devices should switch channel before reading value (like k2700). They must have
  channelCommand. It can be specified in config file like this:
[k2700]
command\channel=ROUT:CLOS(@10%1)

  channelCommand will have %1 substituted to the first value from parameterList. So if we have
[experiment-debug]
out="utime,k2700(5)"

  then k2700 will get
ROUT:CLOS(@105)
  command before reading data from this channel
currentChannel will be set to parameterList.at(0) on successful channel switch.
*/

bool GpibDevice::readValue(QByteArray &returnValue, QStringList parametersList) {
    qDebug()<<"Trying to read value from device"<<shortName();
    bool success=false;
    //if there's no arguments for the device then just use readCommand
    if (parametersList.isEmpty()) {
        success=ask(readCommand.toAscii(),returnValue);
    } else // if there's argument but no channelCommand specified - substitute %1 in readCommand with supplied parameter
        if (parametersList.size()==1 && channelCommand.isEmpty()) {
            qDebug()<<shortName()<<"Reading data with argument. Request:"<<readCommand.arg(parametersList.at(0));
            success=ask(readCommand.arg(parametersList.at(0)).toAscii(),returnValue);
        } else // if there's parameter and channelCommand specified
            if (parametersList.size()==1 && !channelCommand.isEmpty()) {
                if (currentChannel!=parametersList.at(0)) {
                    QByteArray tmpanswer;
                    bool channelSwitchSuccess=ask(channelCommand.arg(parametersList.at(0)).toAscii(),tmpanswer);
                    if (channelSwitchSuccess) {
                        qDebug()<<"Switched channel of device"<<shortName()<<"to"<<parametersList.at(0)<<
                                  "with"<<channelCommand.arg(parametersList.at(0))<<". Answer:"<<tmpanswer;
                        currentChannel=parametersList.at(0);
                    } else {
                        qWarning()<<"Failed to switch channel of device"<<shortName()<<"with command"<<channelCommand.arg(parametersList.at(0));
                        return false;
                    }

                }
                //channel is switched or doesn't required switching. now we can read data
                success=ask(readCommand.toAscii(),returnValue);
            }


    if (success) {
        qDebug()<<"Value"<<returnValue<<"read from"<<shortName();
        returnValue=returnValue.trimmed();
        if (returnValue.isEmpty()) returnValue="-0.000";
        if (getFactor() == 0.0 || getFactor() == 1.0) {
            //qDebug()<<"No factor for"<<shortname;
        } else {
            qDebug()<<"Factor for"<<shortName()<<getFactor();
            returnValue=QByteArray::number(returnValue.toDouble()*getFactor());
        }

        lastValue=returnValue;
        //qDebug()<<"Gpib device id"<<Id()<<"handle"<<Handle()<<"channel"<<channel<<"value"<<returnValue;
        return true;
    } else {
        qWarning()<<"Failed to readValue("<<parametersList<<")";
        return false;
    }

}

/// This function checks if device is online. Returns true on success.
bool GpibDevice::isOnline() {
#ifdef WIN32
    short listen;
    ibln(handle,id,NO_SAD,&listen);
    if (1==listen) {
        qDebug()<<"Device"<<Id()<<"handle"<<Handle()<<"is ONLINE";
        return true;
    }
    qWarning()<<"Device"<<Id()<<"handle"<<Handle()<<"is OFFLINE";
    return false;
#endif
    return true;
}

void GpibDevice::resetDevice() {
    QByteArray query("*RST");
    if (! set (query)) {
        qWarning()<<"Gpib device"<<Id()<<"handle"<<Handle()<<"Failed to ibwrt *RST command";
    }
    qDebug()<<"Gpib device"<<Id()<<"handle"<<Handle()<<"was RESET";
}
