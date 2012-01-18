#include "experimentdata.h"

ExperimentData::ExperimentData(QObject *parent) :
    QAbstractTableModel(parent)
{

}

QVariant ExperimentData::data(const QModelIndex &index, int role) const {

    if (dataTable.isEmpty() || role!=Qt::DisplayRole) {
        return QVariant();
    }
    QVariant val(QVariant::Invalid);
    if (index.column()<dataTable.size() || index.row()<dataTable.at(0).size()) {
        val=this->dataTable.at(index.column()).at(index.row());
    }
    return val;
}

int ExperimentData::columnCount(const QModelIndex &parent) const {
    int val=0;
    if (parent.isValid()) return 0;
    val=dataTable.size();
    return val;
}

int ExperimentData::rowCount(const QModelIndex &parent) const {
    int val=0;
    if (parent.isValid()) return 0;
    if (dataTable.isEmpty()) return 0;
    val=dataTable.at(0).size();
    return val;
}

Qt::ItemFlags ExperimentData::flags(const QModelIndex &index) const {
    if (!index.isValid())
             return Qt::ItemIsEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void ExperimentData::parseLine(QByteArray &line) {
    if (line.startsWith('#') || line.startsWith('*') || line.startsWith("//")) {
        qDebug()<<"Parsing comment\n"<<line;
        parseComment(line);
    } else if (line.startsWith('0') ||
               line.startsWith('1') ||
               line.startsWith('2') ||
               line.startsWith('3') ||
               line.startsWith('4') ||
               line.startsWith('5') ||
               line.startsWith('6') ||
               line.startsWith('7') ||
               line.startsWith('8') ||
               line.startsWith('9')) {
        qDebug()<<"Parsing data\n"<<line;
        parseData(line);
    }
    else {
        qWarning()<<"Failed to parse line"<<line;
    }
}

void ExperimentData::parseData(QByteArray &dataLine) {
    asciiTable.append(dataLine);
    double tmpdouble;
    bool ok=false;
    QVector<double> tmpdoublelist;
    QList<QByteArray> valarray;
    valarray=dataLine.split('\t');

    if (dataTable.isEmpty() && valarray.size()>1 ) {
        //this is a _first_ data portion. dataTable must be prepared
        //and filled as well
        for (int i=0;i<valarray.size();i++) {
            tmpdoublelist.clear();
            tmpdouble=valarray.at(i).toDouble(&ok);
            if (ok) {
                //QList<QList<T> > should be filled with QList<T>
                tmpdoublelist.append(tmpdouble);
                dataTable.append(tmpdoublelist);
            } else {
                qWarning()<<"Initial column"<<i<<"is NaN";
                tmpdoublelist.append(0);
                dataTable.append(tmpdoublelist);
            }
        }

        //inform plot that we have data
        emit newDataAvailable();

        qDebug()<<"Init dataTable with values\n"<<dataTable.size()<<'x'
                <<dataTable.at(0).size();
    } // If dataTable is already initialized then we should fill it
      // only if number of values in the string equals to number of columns in the dataTable
    else if (valarray.size()==dataTable.size()) {
        for (int i=0;i<valarray.size() && i<dataTable.size();i++) {
            tmpdouble=valarray.at(i).toDouble(&ok);
            dataTable[i].append(tmpdouble);
            if (!ok) {
                qWarning()<<"column"<<i<<"is NaN in"<<dataLine<<". Used 0 value";
            }
        }
        // inform plot that we have new data now
        emit newDataAvailable();

    } else {
        qWarning()<<"Number of columns in line"<<dataLine<<"don't match number of columns of dataTable:"<<
                     dataTable.size();
    }

}

void ExperimentData::parseComment(QByteArray &commentLine) {
    asciiTable.append(commentLine);
    qDebug()<<"FIXME: parse comment not implemented";
    qDebug()<<commentLine;
}

void ExperimentData::readInitialData(QByteArray &initialData) {
    //FIXME: this algorithm is memory consuming. Anybody cares?
    //Better solution is to read lines to buf from initialData
    QList<QByteArray> array=initialData.split('\n');
    for (int i=0;i<array.size();i++) {
        parseLine(array[i]);
    }
}

void ExperimentData::resetData() {
    for (int i=0;i<dataTable.size();i++) {
        dataTable[i].clear();
    }
    dataTable.clear();
    qDebug()<<"dataTable cleared";
}

const QStringList & ExperimentData::getAscii() const
{
    return asciiTable;
}
