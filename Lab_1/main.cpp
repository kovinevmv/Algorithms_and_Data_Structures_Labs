#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon("D:/Downloads/2.png"));
    w.show();
    w.setWindowTitle("Лабораторная работа №1");

    return a.exec();
}
