#include "experimentdata.h"
#include "dilatometerdata.h"
#include "plot.h"
#include <QPointF>

ExperimentData::ExperimentData(QObject *parent) :
    QAbstractTableModel(parent)
{
//set expect variable to none (for comment parsing)
    initialUtime=0.0;
    expect=expectNone;
    dilatometerData=NULL;
    plot=NULL;
}

QVariant ExperimentData::data(const QModelIndex &index, int role) const {

    if (role!=Qt::DisplayRole || dataTable.isEmpty() ) {
        return QVariant(QVariant::Invalid);
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

QVariant ExperimentData::headerData ( int section, Qt::Orientation orientation, int role) const {
    if ((role!=Qt::DisplayRole && role!=Qt::ToolTipRole) || dataTable.isEmpty()) {
        return QVariant();
    }

    QVariant val(QVariant::Invalid);
    if (orientation==Qt::Vertical && role==Qt::DisplayRole && section<dataTable.at(0).size() && section>=0) {
        val=section;
    }
    if (orientation==Qt::Horizontal && role==Qt::DisplayRole && section<dataTable.size() && section>=0) {
        val=getColumnLabel(section)+", "+getColumnUnit(section);
    }

    if (orientation==Qt::Horizontal && role==Qt::ToolTipRole && section<dataTable.size() && section>=0) {
        val="Device: "+getColumnShortname(section)+"\n("+
            QString::number(getColumnMin(section))+" - "+
            QString::number(getColumnMax(section))+")";
    }


    return val;

}


void ExperimentData::parseLine(QByteArray &line) {

    if (line.isEmpty()) {
        setExpect(expectNone);
    }


    if (getExpect()==expectDilatometryData) {
        if (dilatometerData) {
            dilatometerData->parseLine(line);
        }
        return;
    }
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

        if (getExpect()==expectExperimentData || getExpect()==expectNone) {
            parseData(line);
        }
    }
    else {
        qWarning()<<"Failed to parse line"<<line;
    }
}

// Notice: This function expect trimmed dataLine
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
            if (utimeColumn==i && ok) {
                initialUtime=tmpdouble;
                qDebug()<<"Initial utime is"<<initialUtime;
                tmpdouble=0.0;
            }
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

        // inform that we have initial data and curves can be created!
        emit initialized();
        //inform plot that we have new data
        emit newDataAvailable();

        qDebug()<<"Init dataTable with values\n"<<dataTable.size()<<'x'
                <<dataTable.at(0).size();
    } // If dataTable is already initialized then we should fill it
      // only if number of values in the string equals to number of columns in the dataTable
    else if (valarray.size()==dataTable.size()) {
        for (int i=0;i<valarray.size() && i<dataTable.size();i++) {
            tmpdouble=valarray.at(i).toDouble(&ok);
            if (utimeColumn==i && initialUtime>0) {
                tmpdouble=tmpdouble-initialUtime;
                if (tmpdouble<0) {
                    qWarning()<<"Initial utime was"<<initialUtime<<"but current time is"<<valarray.at(i);
                }
            }
            dataTable[i].append(tmpdouble);
            if (!ok) {
                qWarning()<<"column"<<i<<"is NaN in"<<dataLine<<". Used 0 value";
            }
        }
        // inform plot that we have new data now
        emit newDataAvailable();

        //inform that we have a number of points currently
        emit pointCount(dataTable.at(0).size());


    } else {
        qWarning()<<"Number of columns in line"<<dataLine<<"don't match number of columns of dataTable:"<<
                     dataTable.size();
    }

}

void ExperimentData::parseComment(QByteArray &commentLine) {
    commentLine=commentLine.trimmed();
    asciiTable.append(QString::fromUtf8(commentLine.data(),commentLine.size()));

    if (getExpect()==expectDevice ||
        getExpect()==expectLabel ||
        getExpect()==expectUnit ||
        getExpect()==expectMin ||
        getExpect()==expectMax ||
        getExpect()==expectAxisHint) {

        //trim the first symbol ('#')
        commentLine=commentLine.right(commentLine.size()-1);
        QList<QByteArray> arr;
        arr=commentLine.split('\t');
        for (int i=0;i<arr.size();i++) {
            if (getExpect()==expectDevice) {
            setColumnShortname(i,arr[i]);
        } else if (getExpect()==expectLabel) {
            setColumnLabel(i,arr[i]);
        } else if (getExpect()==expectUnit) {
            setColumnUnit(i,arr[i]);
        } else if (getExpect()==expectMin) {
            setColumnMin(i,arr[i].toDouble());
        } else if (getExpect()==expectMax) {
            setColumnMax(i,arr[i].toDouble());
        } else if (getExpect()==expectAxisHint) {
            setColumnAxis(i,arr[i]);
        }
        }
        setExpect(expectNone);
        return;
    }



    if (commentLine.startsWith("#Device:")) {
        setExpect(expectDevice);
        return;
    }
    if (commentLine.startsWith("#Label:")) {
        setExpect(expectLabel);
        return;
    }

    if (commentLine.startsWith("#Min:")) {
        setExpect(expectMin);
        return;
    }

    if (commentLine.startsWith("#Max:")) {
        setExpect(expectMax);
        return;
    }

    if (commentLine.startsWith("#Unit:")) {
        setExpect(expectUnit);
        return;
    }

    if (commentLine.startsWith("#Axis hint:")) {
        setExpect(expectAxisHint);
        return;
    }

    if (commentLine.startsWith("#Dilatometry data")) {
        setExpect(expectDilatometryData);
        return;
    }

    if (commentLine.startsWith("*")) {
        //trim the first symbol ('*')
        commentLine=commentLine.right(commentLine.size()-1);
        //extract values separated with tab
        //timestamp  [tab]  controlIndex  [tab]  targetValue
        QString tmp(QString::fromUtf8(commentLine));
        QStringList tmpList=tmp.split("\t",QString::SkipEmptyParts);
        if (tmpList.size()<3) {
            qWarning()<<"Failed to parse transition point\n"<<tmp;
            return;
        }
        bool ok;
        //setting timestamp as double (subtracting initial timestamp
        double markerX=tmpList.at(0).toDouble(&ok)-initialUtime;;
        if (!ok) {
            qWarning()<<"Failed to convert timestamp to double";
            return;
        }


        //setting value as double
        double markerY=tmpList.at(2).toDouble(&ok);
        if (!ok) {
            qWarning()<<"Failed to convert target value to double";
            return;
        }
        QPointF point(markerX,markerY);

        int control=tmpList.at(1).toInt(&ok);
        if (!ok) {
            qWarning()<<"Failed to get cotrol index for marker";
            return;
        }
        emit marker(point,control);
        emit marker(rowCount()-1);
    }
}

void ExperimentData::setControlCount(int controlNum) {

}

void ExperimentData::resetData() {
    for (int i=0;i<dataTable.size();i++) {
        dataTable[i].clear();
    }
    dataTable.clear();
    qDebug()<<"dataTable cleared";

    asciiTable.clear();
    qDebug()<<"asciiTable cleared";

    columnLabel.clear();
    columnShortname.clear();
    columnMax.clear();
    columnMin.clear();
    columnUnit.clear();
    columnAxis.clear();
}

const QStringList & ExperimentData::getAscii() const
{
    return asciiTable;
}

void ExperimentData::setExpect(Expect expectation) {
    expect=expectation;
}

ExperimentData::Expect ExperimentData::getExpect() const {
    return expect;
}

void ExperimentData::setColumnShortname(int column, QByteArray &shortname) {
    // Unfortunately, we can not check if column id exceeds dataTable size
    // because dataTable is initialized AFTER parsing comments.
    if (column<0) return;

    //if column is utime then remember it for later use
    if (shortname=="utime") utimeColumn=column;

//if column index can be accessed in columnShortname
    if (column<columnShortname.size()-1) {
        columnShortname[column]=shortname;
    } else //if column is next to last item in columnSHortname
        if (columnShortname.size()==column) {
        columnShortname.append(shortname);
    } else //if we need to append additional values to access it
    {
        columnShortname.append("");
        // call function recursively
        setColumnShortname(column,shortname);
    }
}

void ExperimentData::setColumnLabel(int column, QByteArray &label) {
    if (column<0) return;

//if column index can be accessed in columnLabel
    if (column<columnLabel.size()-1) {
        columnLabel[column]=label;
    } else //if column is next to last item in columnSHortname
        if (columnLabel.size()==column) {
        columnLabel.append(label);
    } else //if we need to append additional values to access it
    {
        columnLabel.append("");
        // call function recursively
        setColumnLabel(column,label);
    }
}

void ExperimentData::setColumnUnit(int column, QByteArray &unit) {
    if (column<0) return;

//if column index can be accessed in columnLabel
    if (column<columnUnit.size()-1) {
        columnUnit[column]=unit;
    } else //if column is next to last item in columnSHortname
        if (columnUnit.size()==column) {
        columnUnit.append(unit);
    } else //if we need to append additional values to access it
    {
        columnUnit.append("");
        // call function recursively
        setColumnUnit(column,unit);
    }
}

void ExperimentData::setColumnMin(int column, double min) {
    if (column<0) return;

//if column index can be accessed in columnLabel
    if (column<columnMin.size()-1) {
        columnMin[column]=min;
    } else //if column is next to last item in columnSHortname
        if (columnMin.size()==column) {
        columnMin.append(min);
    } else //if we need to append additional values to access it
    {
        columnMin.append(0);
        // call function recursively
        setColumnMin(column,min);
    }
}

void ExperimentData::setColumnMax(int column, double max) {
    if (column<0) return;

//if column index can be accessed in columnLabel
    if (column<columnMax.size()-1) {
        columnMax[column]=max;
    } else //if column is next to last item in columnSHortname
        if (columnMax.size()==column) {
        columnMax.append(max);
    } else //if we need to append additional values to access it
    {
        columnMax.append(0);
        // call function recursively
        setColumnMax(column,max);
    }
}

void ExperimentData::setColumnAxis(int column, QByteArray &axis) {
    if (column<0) return;

    //if column index can be accessed in columnLabel
        if (column<columnAxis.size()-1) {
            columnAxis[column]=toAxisId(axis);
        } else //if column is next to last item in columnSHortname
            if (columnAxis.size()==column) {
            columnAxis.append(toAxisId(axis));
        } else //if we need to append additional values to access it
        {
            columnAxis.append(QwtPlot::xTop);
            // call function recursively
            setColumnAxis(column,axis);
        }
    }


QString ExperimentData::getColumnShortname(int column) const {
    if (column<columnShortname.size()) {
        return columnShortname.at(column);
    }
    return "";
}

QString ExperimentData::getColumnLabel(int column) const {
    if (column<columnLabel.size()) {
        return columnLabel.at(column);
    }
    return "";
}

double ExperimentData::getColumnMin(int column) const {
    if (column<columnMin.size()) {
        return columnMin.at(column);
    }
    return 0;
}

double ExperimentData::getColumnMax(int column) const {
    if (column<columnMax.size()) {
        return columnMax.at(column);
    }
    return 0;
}

QString ExperimentData::getColumnUnit(int column) const {
    if (column<columnUnit.size()) {
        return columnUnit.at(column);
    }
    return "";
}

QwtPlot::Axis ExperimentData::getColumnAxis(int column) const {
    if (column<columnAxis.size()) {
        return columnAxis.at(column);
    }
    return QwtPlot::xTop;
}


const QVector<QVector<double> > * ExperimentData::getDataTable() const {
    return &dataTable;
}

QwtPlot::Axis ExperimentData::toAxisId(const QByteArray &axis) {
    if (axis.startsWith('x')) {
        if (axis=="xTop") return QwtPlot::xTop;
        return QwtPlot::xBottom;
    } else if (axis.startsWith('y')) {
        if (axis=="yRight") return QwtPlot::yRight;
        return QwtPlot::yLeft;
    }
    return QwtPlot::xTop;
}

void ExperimentData::readFile(const QString &filename) {
    if (filename.isEmpty()) {
        qWarning()<<"Tried to read empty file"<<filename;
        return;
    }
    QFile file(filename);

    if (!file.exists()) {
        qWarning()<<"File does not exists"<<file.fileName();
        return;
    }

    if (!file.isReadable()) {
        qWarning()<<"File is not readable"<<file.fileName();
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning()<<"Failed to open file for reading"<<file.fileName();
    }

    //prepare for new data
    resetData();

    QByteArray buf;
    while (!file.atEnd()) {
        buf=file.readLine();
        buf=buf.trimmed();
        parseLine(buf);
    }

    qDebug()<<"Successfully read from file. Closing "<<file.fileName();
    file.close();
}

void ExperimentData::saveFile(const QString &filename) {
    if (filename.isEmpty()) {
        qWarning("File for save is not selected");
        return;
    }

    QFile file(filename);
    if (!file.exists()) {
        qDebug()<<"Will try to create file"<<file.fileName();
    }

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning()<<"Failed to open file for writing"<<file.fileName();
        return;
    }

    QByteArray buf;
    for (int i=0;i<asciiTable.size();i++) {
        buf=asciiTable.at(i).toUtf8();
        buf.append("\n");
        file.write(buf);
        qDebug()<<"Writing to file line"<<i;
    }

    if (dilatometerData) {
        qDebug()<<"Writing dilatometer data to file";
        dilatometerData->saveToFile(file);
    }

    QList<const QwtPlotCurve *> approximationCurveList=plot->getApproximationCurves();
    for (int i=0;i<approximationCurveList.size();++i) {
        const QwtPlotCurve *curve=approximationCurveList.at(i);
        buf="\n#Approximation curve:\n";
        buf+="#Axis hint:\n#xBottom\t";
        if (curve->yAxis()==QwtPlot::yLeft) {
            buf+="yLeft\n";
        } else {
            buf+="yRight\n";
        }
        file.write(buf);
        for (int n=0;n<curve->dataSize();++n) {
            buf=QByteArray::number(curve->sample(n).x(),'g',9);
            buf+="\t"+QByteArray::number(curve->sample(n).y(),'g',9)+"\n";
            qDebug()<<"Writing data line of approximation curve to file";
            file.write(buf);
        }
        file.write("\n");
    }

  file.close();
}


void ExperimentData::setDilatometerData(DilatometerData *dilatometerData) {
    this->dilatometerData=dilatometerData;
}
