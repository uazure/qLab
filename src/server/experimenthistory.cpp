#include "experimenthistory.h"

ExperimentHistory::ExperimentHistory()
{
}

void ExperimentHistory::addHistoryEntry(QString value, int controlIndex)
{
    targetValues.append(value);
    controlIndicies.append(controlIndex);
    timestamps.append(QDateTime::currentDateTime());
}

void ExperimentHistory::clear()
{
    targetValues.clear();
    controlIndicies.clear();
    timestamps.clear();
}

QStringList ExperimentHistory::getHistory() const
{
    QStringList retval;
    for (int i=0;i<targetValues.size();++i) {
        QString tmp=QString::number(timestamps.at(i).toTime_t());
        tmp.append(".");
        tmp.append(timestamps.at(i).toString("zzz"));
        tmp.append("\t");
        tmp.append(targetValues.at(i)).append("\t");
        tmp.append("control").append(" ").append(QString::number(controlIndicies.at(i)));
        retval.append(tmp);
    }
    return retval;
}
