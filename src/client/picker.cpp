#include "picker.h"

Picker::Picker(QwtPlot *plot) :
    QwtPicker(plot)
{
    QwtPlotCanvas *canvas = plot->canvas();
    canvas->installEventFilter(this);

    // We want the focus, but no focus rect. The
    // selected point will be highlighted instead.
    canvas->setFocusPolicy(Qt::StrongFocus);
#ifndef QT_NO_CURSOR
    canvas->setCursor(Qt::PointingHandCursor);
#endif
    canvas->setFocusIndicator(QwtPlotCanvas::ItemFocusIndicator);
    canvas->setFocus();
}

bool Picker::eventFilter(QObject *object, QEvent *e) {
    if ( object != (QObject *)plot()->canvas() )
        return false;

    switch(e->type()) {
    case QEvent::FocusIn:
        //qDebug()<<"Focus in";
        //showCursor(true);
    case QEvent::FocusOut:
        //qDebug()<<"Focus out";
        //showCursor(false);
    case QEvent::Paint:
    {
        qDebug()<<"Paint event";
        //QApplication::postEvent(this, new QEvent(QEvent::User));
        break;
    }
    case QEvent::MouseButtonPress:
    {
        QMouseEvent *mouse = ((QMouseEvent *)e);
        if (mouse->button()!=Qt::LeftButton)
            return false;
        if (mouse->modifiers()==Qt::NoModifier) {
            qDebug("select NO MODIFIERS");
            emit selectSignle(mouse->pos());
        } else if (mouse->modifiers()&Qt::SHIFT) {
            qDebug("select SHIFT");
            emit selectRange(mouse->pos());
        } else if (mouse->modifiers()&Qt::CTRL) {
            qDebug("select CTRL");
            emit appendSingle(mouse->pos());
        } else if (mouse->modifiers()&Qt::ALT) {
            qDebug("select ALT");
        }
        return true;
    }

    default:
        break;
    }

    return QObject::eventFilter(object, e);
}

bool Picker::event(QEvent *e) {
    if ( e->type() == QEvent::User )
        {
            qDebug("User event");
            //Mark currently selected point
            //showCursor(true);
            return true;
        }
        return QObject::event(e);
}




