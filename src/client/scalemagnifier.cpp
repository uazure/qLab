#include "scalemagnifier.h"

ScaleMagnifier::ScaleMagnifier(QWidget *parent) :
    QwtMagnifier(parent)
{
    setMouseButton(Qt::LeftButton);
    setMouseFactor(1.05);
    setWheelFactor(1.414);
}

bool ScaleMagnifier::eventFilter(QObject *object, QEvent *event) {
    //if ( object && object == parent() )
    if ( object )
    {
        switch ( event->type() )
        {
        case QEvent::MouseButtonPress:
        {
            widgetMousePressEvent( ( QMouseEvent * )event );
            break;
        }
        case QEvent::MouseMove:
        {
            widgetMouseMoveEvent( ( QMouseEvent * )event );
            break;
        }
        case QEvent::MouseButtonRelease:
        {
            widgetMouseReleaseEvent( ( QMouseEvent * )event );
            break;
        }
        case QEvent::Wheel:
        {
            widgetWheelEvent( ( QWheelEvent * )event );
            break;
        }
        case QEvent::KeyPress:
        {
            widgetKeyPressEvent( ( QKeyEvent * )event );
            break;
        }
        case QEvent::KeyRelease:
        {
            widgetKeyReleaseEvent( ( QKeyEvent * )event );
            break;
        }
        default:;
        }
    }
    return QObject::eventFilter( object, event );
}
