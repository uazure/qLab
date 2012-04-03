#include "dilatometerdata.h"
#include <QSettings>
#include <QDebug>
#include "thermalexpansionpoint.h"


DilatometerData::DilatometerData(QObject *parent) :
    QObject(parent)
{
    QSettings settings;
    bool ok;
    L0=settings.value("dilatometer/L0",QVariant(1.0)).toDouble(&ok);
    if (!ok) {
        qWarning("Failed read value of L0 from settings. L0=1; Change it manually and save");
        L0=1;
    }


}


