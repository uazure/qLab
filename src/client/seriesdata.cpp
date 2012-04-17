#include "seriesdata.h"


SeriesData::SeriesData(const QVector<QVector<double> > *data, int y, int x)
{
    dataTable=data;
    xCol=x;
    yCol=y;
    lastBoundSize=0;
}

SeriesData::~SeriesData() {

}

size_t SeriesData::size() const {
    if (dataTable->size()==0) return 0;
    return dataTable->at(0).size();
}

QPointF SeriesData::sample(size_t i) const {
    //qDebug()<<"Requested sample"<<i;
    if (dataTable->size()==0) return QPointF();

    if (i>dataTable->at(0).size()) {
        return QPointF();
    }
    QPointF point(dataTable->at(xCol).at(i),dataTable->at(yCol).at(i));
    //qDebug()<<"Point:"<<point;
    return point;
}


QRectF SeriesData::boundingRect() const {
    if (lastBoundSize==size()) {
        qDebug()<<"Series size unchanged, returning cached boundary";
        return d_boundingRect;
    }

    if (d_boundingRect.isValid() && lastBoundSize<size()) {
        qDebug()<<"Series size exceeds cached boundary size. Extending"<<d_boundingRect;
        for (int i=lastBoundSize;i<size();++i) {
            QPointF p=sample(i);
            if (!d_boundingRect.contains(p)) {
                //try to expand rect to right-left-top-bottom
                if (d_boundingRect.right()<p.x()) {
                    d_boundingRect.setRight(p.x());
                }
                if (d_boundingRect.left()>p.x()) {
                    d_boundingRect.setLeft(p.x());
                }
                if (d_boundingRect.top()>p.y()) {
                    qDebug()<<"top:"<<d_boundingRect.top();
                    d_boundingRect.setTop(p.y());
                }
                if (d_boundingRect.bottom()<p.y()) {
                    qDebug()<<"bottom:"<<d_boundingRect.bottom();
                    d_boundingRect.setBottom(p.y());
                }
            }
        }


    if (d_boundingRect.isValid()) {
        qDebug()<<"Extended boundary valid:"<<d_boundingRect;
        lastBoundSize=size();
        return d_boundingRect;
        }

    }

//if none of the above triggered then recalculate lastBound
    qDebug()<<"Recalculating boundary";
    lastBoundSize=size();
    d_boundingRect=qwtBoundingRect(*this);
    return d_boundingRect;
}




