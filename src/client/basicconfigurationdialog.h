#ifndef BASICCONFIGURATIONDIALOG_H
#define BASICCONFIGURATIONDIALOG_H

#include <QSettings>
#include <QDialog>
#include <QAbstractButton>
#include <QDoubleValidator>

namespace Ui {
    class BasicConfigurationDialog;
}

class BasicConfigurationDialog : public QDialog {
    Q_OBJECT
public:
    BasicConfigurationDialog(QWidget *parent = 0);
    ~BasicConfigurationDialog();

private slots:
    void buttonClicked(QAbstractButton *but);
    void saveSettings();

protected:
    void changeEvent(QEvent *e);
    QSettings settings;

private:
    Ui::BasicConfigurationDialog *ui;
    QDoubleValidator *doubleValidator;
};

#endif // BASICCONFIGURATIONDIALOG_H
