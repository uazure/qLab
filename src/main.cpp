#include <QtGui/QApplication>
#include <QSettings>
#include <stdio.h>
#include <stdlib.h>

#include "mainwindow.h"



 void myMessageOutput(QtMsgType type, const char *msg)
 {
     switch (type) {
     case QtDebugMsg:
         fprintf(stderr, "Debug: %s\n", msg);
         break;
     case QtWarningMsg:
         fprintf(stderr, "Warning: %s\n", msg);
         break;
     case QtCriticalMsg:
         fprintf(stderr, "Critical: %s\n", msg);
         break;
     case QtFatalMsg:
         fprintf(stderr, "Fatal: %s\n", msg);
         abort();
     }
 }



int main(int argc, char *argv[])
{
    qInstallMsgHandler(myMessageOutput);
    QApplication a(argc, argv);
    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

    a.setOrganizationName("qLab");
    a.setApplicationName("qLab server");
    a.setApplicationVersion("0.0.1 develop");

    MainWindow w;
    w.show();

    return a.exec();
}
