#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "devicefarm.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void setFullscreen(bool isFullscreen);
    void showAboutQt(void);

};

#endif // MAINWINDOW_H
