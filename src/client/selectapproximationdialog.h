#ifndef SELECTAPPROXIMATIONDIALOG_H
#define SELECTAPPROXIMATIONDIALOG_H

#include <QDialog>
#include <QDebug>

namespace Ui {
    class SelectApproximationDialog;
}

class SelectApproximationDialog : public QDialog {
    Q_OBJECT
public:
    SelectApproximationDialog(const QString &name,QWidget *parent = 0);
    ~SelectApproximationDialog();
    int getApproximationMethodId() const;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SelectApproximationDialog *ui;
};

#endif // SELECTAPPROXIMATIONDIALOG_H
