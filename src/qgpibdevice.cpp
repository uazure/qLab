#include "qgpibdevice.h"

QGpibDevice::QGpibDevice(QByteArray &DeviceShortName, QObject *parent) :
        QAbstractDevice (parent)

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
    }
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
    qDebug()<<"Device"<<shortname<<"channel command is"<<channelCommand;


}

QGpibDevice::~QGpibDevice() {
    delete deviceSettings;
    delete experimentSettings;
}

/// Get identification string from the device
bool QGpibDevice::getIdn() {
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
int QGpibDevice::Id() const {
    return id;
}

/// Returns gpib handle of the device
int QGpibDevice::Handle() const {
    return handle;
}

/// Write data to the gpib device. Returns true on success or false on failure. Emits errorMessage on error.
bool QGpibDevice::set(QString command) {
#ifdef WIN32
    ibwrt(this->handle,command.data(),command.size());
    if (ibsta&ERR) {
        qDebug()<<"Gpib error: ibwrt failed on device" <<shortName()<<"with id"<<Id();
        emit errorMessage("Gpib error: ibwrt failed");
        return false;
    }
#endif
    return true;
}

/// Read data from the gpib device. Returns true on success or false on failure.
bool QGpibDevice::get(QByteArray &reply) {
#ifdef WIN32
    char tmp[255];
    ibrd(Handle(),tmp,255);
    if (ibsta&ERR) {
        qDebug()<<"Gpib error: ibrd failed on device"<<shortName()<<"with id"<<Id();
        emit errorMessage("Gpib error: ibrd failed");
        return false;
    }
    reply.append(tmp);
#endif
    return true;
}

/// Handful method for asking gpib device for value. Returns true on success or false on failure.
bool QGpibDevice::ask(QString command, QByteArray &reply) {
    if (!set(command)) {
        return false;
    }

    if (!get(reply)) {
        return false;
    }
    return true;
}

/// Implementation of QAbstractDevice virtual primary method for gpib deivce
/** FIXME: This requres to read configuration for each device to know
which commands to issue to the device. */
bool QGpibDevice::readValue(QByteArray &returnValue, QStringList parametersList) {
    if (ask(readCommand,returnValue)) {
        returnValue=returnValue.trimmed();
        if (getFactor() == 0 || getFactor() == 1) {
            qDebug()<<"No factor for"<<shortname;
        } else {
            qDebug()<<"Factor for"<<shortName()<<getFactor();
            returnValue=QByteArray::number(returnValue.toDouble()*getFactor());
        }

        //qDebug()<<"Gpib device id"<<Id()<<"handle"<<Handle()<<"channel"<<channel<<"value"<<returnValue;
        return true;
    }
    return false;
}

/// This function checks if device is online. Returns true on success.
bool QGpibDevice::isOnline() {
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

void QGpibDevice::resetDevice() {
    QByteArray query("*RST");
    if (! set (query)) {
        qWarning()<<"Gpib device"<<Id()<<"handle"<<Handle()<<"Failed to ibwrt *RST command";
    }
    qDebug()<<"Gpib device"<<Id()<<"handle"<<Handle()<<"was RESET";
}
