#include <QtGui/QApplication>
#include <QSettings>


#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

    a.setOrganizationName("qLab");
    a.setApplicationName("qlab");

    MainWindow w;
    w.show();

    return a.exec();
}
