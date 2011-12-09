#include "qgpibdevice.h"

QGpibDevice::QGpibDevice(short gpib_id, QObject *parent) :
    QAbstractDevice (Gpib, parent)

{
    id=gpib_id;


}

/// Get identification string from the device
bool QGpibDevice::getIdn() {

    QByteArray query("IDN?");
    QByteArray response;
    if (ask (query,&response)) {
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
    ibwrt(this->handle,command.data(),command.size()+1);
    if (ibsta&ERR) {
        qDebug()<<"Gpib error: ibwrt failed on device" <<shortName()<<"with id"<<Id();
        emit errorMessage("Gpib error: ibwrt failed");
        return false;
    }
    return true;
}

/// Read data from the gpib device. Returns true on success or false on failure.
bool QGpibDevice::get(QByteArray *reply) {
    char tmp[255];
    ibrd(Handle(),tmp,255);
    if (ibsta&ERR) {
        qDebug()<<"Gpib error: ibrd failed on device"<<shortName()<<"with id"<<Id();
        emit errorMessage("Gpib error: ibrd failed");
        return false;
    }
    reply->append(tmp);
    return true;
}

/// Handful method for asking gpib device for value. Returns true on success or false on failure.
bool QGpibDevice::ask(QByteArray command, QByteArray *reply) {
    if (!set(command)) {
        return false;
    }

    if (!get(reply)) {
        return false;
    }
    return true;
}
