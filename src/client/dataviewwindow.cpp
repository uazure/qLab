#include "dataviewwindow.h"
#include "ui_dataviewwindow.h"

DataViewWindow::DataViewWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataViewWindow)
{
    ui->setupUi(this);
}

DataViewWindow::~DataViewWindow()
{
    delete ui;
}

void DataViewWindow::setAscii(const QStringList &ascii)
{
    ui->listWidget->addItems(ascii);
}

void DataViewWindow::setItem(QAbstractItemModel *model) {
    ui->tableView->setModel(model);
}
