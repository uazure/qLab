#include <QtWidgets/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("qLab");
    a.setApplicationName("qlab-client");
    a.setApplicationVersion("0.0.3");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    QStringList arguments=a.arguments();
    QString filename;
    if (arguments.size()>1)
        filename=arguments.at(1);

    MainWindow *window = new MainWindow(filename);

    window->show();

    return a.exec();
}
