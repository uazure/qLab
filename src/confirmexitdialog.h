#ifndef CONFIRMEXITDIALOG_H
#define CONFIRMEXITDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
    class ConfirmExitDialog;
}

class ConfirmExitDialog : public QDialog {
    Q_OBJECT
public:
    ConfirmExitDialog(QWidget *parent = 0);
    ~ConfirmExitDialog();

public slots:
    void setQuestion(QString question);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ConfirmExitDialog *ui;
};

#endif // CONFIRMEXITDIALOG_H
