#include "qgpibdevice.h"

QGpibDevice::QGpibDevice(QByteArray &DeviceShortName, QObject *parent) :
    QAbstractDevice (Gpib, parent)

{
    // initialising gpib external variables;
    ibsta=0x11;
    iberr=0x22;
    ibcnt=0x33;
    ibcntl=0x44;

    shortname=DeviceShortName;
    settings=new QSettings(QSettings::IniFormat,QSettings::UserScope,
        QApplication::organizationName(),"devices");

    id=settings->value(shortname.append("/id"),0).toInt();
    if (!id) {
        qWarning()<<"Device:"<<shortname<<"id read from config file is"<<id;
    }

    QStringList initStringList=settings->value(shortname.append("/command/init")).toStringList();
    for (int i=0;i<initStringList.size()-1;i++) {
        if (! set (initStringList.at(i).toLocal8Bit())) {
            qWarning()<<"Device"<<shortname<<"init with"<<initStringList.at(i)<<"failed";
        } else {
            qDebug()<<"Device"<<shortname<<"init with"<<initStringList.at(1)<<"OK";
        }
    }

// define handle to speak to device using ibwrt/ibrd functions
    handle=ibdev(0,id,NO_SAD,T300ms,1,0);
        if (ibsta&ERR)
    {
        qDebug()<<"Gpib error: ibdev failed on device"<<shortName()<<"with id"<<Id();
        emit errorMessage("Gpib error: ibdev failed");
    }

    if (isOnline()) {
        this->getIdn();
    }

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
bool QGpibDevice::set(QByteArray command) {
    ibwrt(this->handle,command.data(),command.size());
    if (ibsta&ERR) {
        qDebug()<<"Gpib error: ibwrt failed on device" <<shortName()<<"with id"<<Id();
        emit errorMessage("Gpib error: ibwrt failed");
        return false;
    }
    return true;
}

/// Read data from the gpib device. Returns true on success or false on failure.
bool QGpibDevice::get(QByteArray &reply) {
    char tmp[255];
    ibrd(Handle(),tmp,255);
    if (ibsta&ERR) {
        qDebug()<<"Gpib error: ibrd failed on device"<<shortName()<<"with id"<<Id();
        emit errorMessage("Gpib error: ibrd failed");
        return false;
    }
    reply.append(tmp);
    return true;
}

/// Handful method for asking gpib device for value. Returns true on success or false on failure.
bool QGpibDevice::ask(QByteArray command, QByteArray &reply) {
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
bool QGpibDevice::readValue(QByteArray &returnValue,int channel) {
    if (ask("DATA?",returnValue)) {
        returnValue=returnValue.trimmed();
        qDebug()<<"Gpib device id"<<Id()<<"handle"<<Handle()<<"channel"<<channel<<"value"<<returnValue;
        return true;
    }
    return false;
}

/// This function checks if device is online. Returns true on success.
bool QGpibDevice::isOnline() {
    short listen;
    ibln(handle,id,NO_SAD,&listen);
    if (1==listen) {
        qDebug()<<"Device"<<Id()<<"handle"<<Handle()<<"is ONLINE";
        return true;
    }
    qWarning()<<"Device"<<Id()<<"handle"<<Handle()<<"is OFFLINE";
    return false;
}

void QGpibDevice::resetDevice() {
    QByteArray query("*RST");
    if (! set (query)) {
        qWarning()<<"Gpib device"<<Id()<<"handle"<<Handle()<<"Failed to ibwrt *RST command";
    }
    qDebug()<<"Gpib device"<<Id()<<"handle"<<Handle()<<"was RESET";
}
