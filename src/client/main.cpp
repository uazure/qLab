#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStringList arguments=a.arguments();
    QString filename;
    if (arguments.size()>1)
        filename=arguments.at(1);

    MainWindow w(filename);
    w.show();

    return a.exec();
}
