#ifndef EXPERIMENTDATA_H
#define EXPERIMENTDATA_H

#include <QAbstractTableModel>
#include <QVector>
#include <QDebug>

class ExperimentData : public QAbstractTableModel
{
Q_OBJECT
public:
    explicit ExperimentData(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    int columnCount(const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    QVector<QVector<double> > dataTable;

signals:


public slots:
    void parseLine(QByteArray &line);
    void parseComment(QByteArray &commentLine);
    void parseData(QByteArray &dataLine);

};

#endif // EXPERIMENTDATA_H
