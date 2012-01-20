#ifndef SERIESDATA_H
#define SERIESDATA_H
#include "qwt_series_data.h"
#include <QPointF>

class SeriesData : public QwtSeriesData<QPointF>
{
public:
    explicit SeriesData(const QVector<QVector<double> > *data,int y, int x=0);
    //void setData ();
    size_t size() const;
    QPointF sample(size_t i) const;
    QRectF boundingRect() const;


private:
    const QVector<QVector<double> > *dataTable;
    int xCol;
    int yCol;


};

#endif // SERIESDATA_H
