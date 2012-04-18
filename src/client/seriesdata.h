#ifndef SERIESDATA_H
#define SERIESDATA_H
#include "qwt_series_data.h"
#include <QPointF>
#include <QObject>

class SeriesData : public QwtSeriesData<QPointF>
{
    //unidentified copy constructor
    SeriesData(const SeriesData&);

public:
    explicit SeriesData(const QVector<QVector<double> > *data,int y, int x=0);
    ~SeriesData();
    //void setData ();
    size_t size() const;
    QPointF sample(size_t i) const;
    QRectF boundingRect() const;

private:
    const QVector<QVector<double> > *dataTable;
    int xCol;
    int yCol;
    mutable int lastBoundSize;
    mutable size_t lastSize;

};

#endif // SERIESDATA_H
