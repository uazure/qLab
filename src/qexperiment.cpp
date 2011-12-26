#include "qexperiment.h"

qExperiment::qExperiment(QString name, QObject *parent) :
    QObject(parent)
{
    this->name=name;
}
