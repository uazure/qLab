#include <QApplication>
#include "mainwindow.h"
#include <QSystemTrayIcon>
#include <QMenu>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    a.setOrganizationName("qLab");
//    a.setApplicationName("devices");


    MainWindow w;
    w.show();
    return a.exec();
}
