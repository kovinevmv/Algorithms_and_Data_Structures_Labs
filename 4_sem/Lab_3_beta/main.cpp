#include "mainwindow.h"
#include <QApplication>
#include <time.h>

int main(int argc, char *argv[])
{
    srand(time(NULL));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setFixedSize(1060, 570);
    w.setWindowTitle("Лабораторная работа №3");

    return a.exec();
}
