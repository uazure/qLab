#ifndef DOUBLECLICKEVENTFILTER_H
#define DOUBLECLICKEVENTFILTER_H

#include <QObject>

class DoubleClickEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit DoubleClickEventFilter(QObject *parent = 0);
    bool eventFilter(QObject *o, QEvent *e);

signals:
    void doubleClicked();

public slots:

};

#endif // DOUBLECLICKEVENTFILTER_H
