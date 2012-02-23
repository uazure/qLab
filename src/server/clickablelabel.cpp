#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget *parent) :
    QLabel(parent)
{
}

void ClickableLabel::mouseDoubleClickEvent(QMouseEvent *e) {
    emit doubleClicked();
}

void ClickableLabel::mouseReleaseEvent(QMouseEvent *ev) {
    if (ev->button()==Qt::LeftButton) {
        emit clicked();
    }
}
