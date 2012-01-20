#include "plot.h"

Plot::Plot(QWidget *parent) :
    QwtPlot(parent)
{

}

Plot::~Plot() {
this->detachItems();
}
