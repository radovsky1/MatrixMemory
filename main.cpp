#include "mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QDesktopWidget *desktop = QApplication::desktop();
    int screenWidth = desktop->width();
    int screenHeight = desktop->height();
    w.screenHeight = screenHeight;
    w.screenWidth = screenWidth;
    w.setWindowTitle("Matrix Memory");
    w.show();
    return a.exec();
}
