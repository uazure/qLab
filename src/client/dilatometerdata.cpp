#include "dilatometerdata.h"
#include <QSettings>
#include <QVariant>
#include <QDebug>
#include "thermalexpansionpoint.h"
#include <QFile>


DilatometerData::DilatometerData(QObject *parent) :
    QObject(parent)
{
    QSettings settings;
    bool ok;
    L0=settings.value("dilatometry/L0",QVariant(1.0)).toDouble(&ok);
    if (!ok) {
        qWarning("Failed read value of L0 from settings. L0=1; Change it manually and save");
        L0=1;
    }
}



bool DilatometerData::setThermalExpansion(double T0, double T1, double dF, double Favg, double tau1, double tau2) {
    try {
        bool onHeat;
        double dL=dF/sensitivity(Favg);
        double dT=T1-T0;
        double Tavg=(T1+T0)/2;
        if (T1>T0) onHeat=true;
        else onHeat=false;
        double alpha=dL/dT*L0;
        ThermalExpansionPoint p(Tavg,alpha,dL,tau1,tau2,onHeat);
        qDebug()<<"Adding new thermal expansion point to set: alpha ="<<alpha<<"T ="<<Tavg<<"dL ="<<dL<<"tau1 ="<<tau1<<"tau2 ="<<tau2<<"onHeat ="<<onHeat;
        thermalExpansionVector.append(p);
        return true;
    } catch (...) {
        qWarning()<<"Failed to calculate alpha";
        return false;
    }
}

void DilatometerData::setL0(double L0) {
    this->L0=L0;
    qDebug()<<"DilatometerData L0 set to"<<L0;
    QSettings settings;
    settings.setValue("dilatometry/L0",QVariant(L0));
    qDebug()<<"L0 set to"<<L0<<"and saved to file";
}

void DilatometerData::deletePoint(int index) {
    if (index<0 || index>=size()) {
        return;
    }
    thermalExpansionVector.remove(index);
}

void DilatometerData::saveToFile(QFile &file) {
    if (!file.isOpen() || !file.isWritable()) {
        qDebug()<<"Can not save dilatometry data to file.";
        return;
    }
    if  (size()==0) {
        qDebug()<<"No dilatometry data to save";
        return;
    }

    QByteArray buf;
    file.write("\n#Dilatometry data:\n");
    file.write("#Temperature\tAlpha\tdeltaL\tTau1\tTau2\tonHeat\n");
    for (int i=0;i<size();++i) {
        buf=QByteArray::number(thermalExpansionVector.at(i).getT(),'g',9)+"\t"+
            QByteArray::number(thermalExpansionVector.at(i).getAlpha(),'g',9)+"\t"+
            QByteArray::number(thermalExpansionVector.at(i).getDeltaL(),'g',9)+"\t"+
            QByteArray::number(thermalExpansionVector.at(i).getTau1(),'g',9)+"\t"+
            QByteArray::number(thermalExpansionVector.at(i).getTau2(),'g',9)+"\t"+
            QByteArray::number((int) thermalExpansionVector.at(i).isOnHeat())+"\n";
        file.write(buf);
    }
}


void DilatometerData::parseLine (const QByteArray &line) {
    if (line.startsWith("#")) return;
    QList<QByteArray> array;
    array=line.split('\t');
    if (array.size()<6) {
        qWarning()<<"Expected 6 columns for dilatometry data, got"<<array.size()<<"skipping";
        return;
    }

    bool onHeat=false;
    if (array.at(5).toInt()) onHeat=true;

    ThermalExpansionPoint p(array.at(0).toDouble(),
                            array.at(1).toDouble(),
                            array.at(2).toDouble(),
                            array.at(3).toDouble(),
                            array.at(4).toDouble(),
                            onHeat);
    qDebug()<<"Appended thermal expansion point"<<p;
    thermalExpansionVector.append(p);
}

