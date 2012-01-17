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
    if (!parent.isValid()) return 0;
    val=dataTable.size();
    return val;
}

int ExperimentData::rowCount(const QModelIndex &parent) const {
    int val=0;
    if (!parent.isValid()) return 0;
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
}

void ExperimentData::parseData(QByteArray &dataLine) {
    double tmpdouble;
    bool tmpbool=false;
    QVector<double> tmpdoublelist;
    QList<QByteArray> valarray;
    valarray=dataLine.split('\t');

    if (dataTable.isEmpty() && valarray.size()>1 ) {
        //this is a _first_ data portion. ddata must be prepared
        //and filled as well
        for (int i=0;i<valarray.size();i++) {

            tmpdouble=valarray.at(i).toDouble(&tmpbool);
            if (tmpbool) {
                //QList<QList<T> > should be filled with QList<T>
                tmpdoublelist.clear();
                tmpdoublelist.append(tmpdouble);
                dataTable.append(tmpdoublelist);
            } else {
                qCritical()<<"Critical: row 0, column"<<i<<"is NaN";
                dataTable.clear();
                return;
            }
        }
        qDebug()<<"Init dataTable with values\n"<<dataTable.size()<<'x'
                <<dataTable.at(0).size();
    }

}

void ExperimentData::parseComment(QByteArray &commentLine) {
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
