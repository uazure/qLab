#ifndef STRINGLISTSELECTDIALOG_H
#define STRINGLISTSELECTDIALOG_H

#include <QDialog>
#include <QStringList>

namespace Ui {
    class StringListSelectDialog;
}

class StringListSelectDialog : public QDialog {
    Q_OBJECT
public:
    StringListSelectDialog(const QString &title, const QString &text, const QStringList &list,QWidget *parent = 0);
    ~StringListSelectDialog();
    //returns index of selected item
    int getSelectedIndex() const;



protected:
    void changeEvent(QEvent *e);

private:
    Ui::StringListSelectDialog *ui;
};

#endif // STRINGLISTSELECTDIALOG_H
