#include "seriesdata.h"


SeriesData::SeriesData(const QVector<QVector<double> > *data, int y, int x)
{
    dataTable=data;
    xCol=x;
    yCol=y;
}

SeriesData::~SeriesData() {

}

size_t SeriesData::size() const {
    if (dataTable->size()==0) return 0;
    return dataTable->at(0).size();
}

QPointF SeriesData::sample(size_t i) const {
    qDebug()<<"Requested sample"<<i;
    if (dataTable->size()==0) return QPointF();

    if (i>dataTable->at(0).size()) {
        return QPointF();
    }
    QPointF point(dataTable->at(xCol).at(i),dataTable->at(yCol).at(i));
    qDebug()<<"Point:"<<point;
    return point;
}

    QRectF SeriesData::boundingRect() const {
    return qwtBoundingRect(*this);
}




