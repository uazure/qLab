#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QMouseEvent>

class ClickableLabel : public QLabel
{
Q_OBJECT
public:
    explicit ClickableLabel(QWidget *parent = 0);

private:
   void mouseDoubleClickEvent(QMouseEvent *);
   void mouseReleaseEvent(QMouseEvent *ev);

signals:
    void clicked();
    void doubleClicked();

public slots:

};

#endif // CLICKABLELABEL_H
