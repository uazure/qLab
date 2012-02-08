#include "scaleplotpanner.h"

ScalePlotPanner::ScalePlotPanner(QwtPlotCanvas *canvas):
        QwtPlotPanner(canvas)
{
}


bool ScalePlotPanner::eventFilter(QObject *object, QEvent *event) {

    if ( object == NULL )
        return false;

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
    case QEvent::Paint:
        {
            if ( isVisible() )
                return true;
            break;
        }
    default:;
    }

    return false;
}
