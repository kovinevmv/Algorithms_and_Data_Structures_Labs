#include "mainwindow.h"
#include <QApplication>
#include "time.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication a(argc, argv);
    MainWindow w;

    w.setFixedHeight(600);
    w.setFixedWidth(650);
    w.setWindowTitle("Лабораторная Работа №3");
    w.show();
    return a.exec();
}
