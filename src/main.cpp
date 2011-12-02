#include <QtGui/QApplication>
#include <QSystemTrayIcon>
#include <QMenu>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("qLab");
    a.setApplicationName("qlab");

//    QSystemTrayIcon *trayIcon=new QSystemTrayIcon();
//    QMenu trayMenu;
//    trayMenu.addMenu("Exit");
//    trayIcon.setContextMenu(&trayMenu());
//    trayIcon.show();

    MainWindow w;
    w.show();

    return a.exec();
}
