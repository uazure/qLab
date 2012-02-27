#ifndef EXPERIMENTHISTORY_H
#define EXPERIMENTHISTORY_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QTime>
#include <QDateTime>

/** This class is designed to keep history of experiment targets of experiment controls.

  */
class ExperimentHistory
{
public:
    ExperimentHistory();
    void addHistoryEntry(QString value, int controlIndex=0);
    void clear();
    QStringList getHistory() const;

private:
    QList<QString> targetValues;
    QList<int> controlIndicies;
    QList<QDateTime> timestamps;


};

#endif // EXPERIMENTHISTORY_H
