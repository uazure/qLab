#include "doubleclickeventfilter.h"
#include <QEvent>

DoubleClickEventFilter::DoubleClickEventFilter(QObject *parent) :
    QObject(parent)
{
}

bool DoubleClickEventFilter::eventFilter(QObject *o, QEvent *e)
{
    if (!o) {
        return false;
    }
    if (e->type()==QEvent::MouseButtonDblClick) {
        emit doubleClicked();
        return true;
    }
    return false;
}

