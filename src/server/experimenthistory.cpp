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

QStringList ExperimentHistory::getHistory(int controlIndex) const
{
    QStringList retval;


    for (int i=0;i<targetValues.size();++i) {
        if (controlIndex<0 || controlIndicies.at(i)==controlIndex) {
            QString tmp;
            tmp.append(QString::number(controlIndicies.at(i))).append("\t");
            tmp.append(targetValues.at(i));
            retval.append(tmp);
        }
    }
    return retval;
}
