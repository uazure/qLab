#ifndef SCALEMAGNIFIER_H
#define SCALEMAGNIFIER_H
#include "qwt_magnifier.h"
#include <QEvent>

class QwtPlot;

class ScaleMagnifier : public QwtMagnifier
{
    Q_OBJECT
public:
    explicit ScaleMagnifier(QWidget *parent = 0);
    bool eventFilter(QObject *, QEvent *);

signals:

public slots:

};

#endif // SCALEMAGNIFIER_H
