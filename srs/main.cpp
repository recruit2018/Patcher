#include <QApplication>
#include "mainwindow.h"


int main (int argc, char** argv)
{
    QApplication a(argc, argv);

    QApplication::setOrganizationName("Organization");
    QApplication::setApplicationName("Patcher");

    MainWindow win;
    win.show();

    return a.exec();
}

































