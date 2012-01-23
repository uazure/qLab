#ifndef DATAVIEWWINDOW_H
#define DATAVIEWWINDOW_H

#include <QDialog>
#include <QAbstractItemModel>

namespace Ui {
    class DataViewWindow;
}

class DataViewWindow : public QDialog
{
    Q_OBJECT
    //unidentified copy constructor
    DataViewWindow(const DataViewWindow&);

public:
    explicit DataViewWindow(QWidget *parent = 0);
    ~DataViewWindow();
    void setAscii(const QStringList &ascii);
    void setItem(QAbstractItemModel *model);

private:
    Ui::DataViewWindow *ui;
};

#endif // DATAVIEWWINDOW_H
