#ifndef EXPERIMENTDATA_H
#define EXPERIMENTDATA_H

#include <QAbstractTableModel>
#include <QVector>
#include <QDebug>
#include <QStringList>

class ExperimentData : public QAbstractTableModel
{
Q_OBJECT
public:
    explicit ExperimentData(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int columnCount(const QModelIndex &parent=QModelIndex()) const;
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    const QStringList & getAscii() const;

private:
    QVector<QVector<double> > dataTable;
    QStringList asciiTable;

signals:
    void newDataAvailable();


public slots:
    void readInitialData(QByteArray &initialData);
    void resetData(void);
    void parseLine(QByteArray &line);
    void parseComment(QByteArray &commentLine);
    void parseData(QByteArray &dataLine);

};

#endif // EXPERIMENTDATA_H
