#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>
#include <QApplication>
#include <QClipboard>
#include <QKeyEvent>
#include <QDebug>
#include <QMenu>

class TableView : public QTableView
{
Q_OBJECT
public:
    explicit TableView(QWidget *parent = 0);

protected:
    virtual void keyPressEvent(QKeyEvent *event);

signals:

public slots:
    void copy();

private slots:
    void showContextMenu(const QPoint &point);

};

#endif // TABLEVIEW_H
