#include "mainwindow.h"
#include <QApplication>
#include <time.h>
#include <QMainWindow>

int main(int argc, char **argv)
{
    srand(time(NULL));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
   // w.setFixedSize(1100, 730);
    w.setWindowTitle("Лабораторная работа №3");
    w.setWindowIcon(QIcon(":images/images/2.png"));

    return a.exec();
}
