#include "mainwindow.h"
#include <QApplication>
#include <time.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    w.setFixedSize(280, 300);
    w.setWindowIcon(QIcon("2.png"));
    w.setWindowTitle("Лабораторная работа №3");
    srand(time(NULL));
    return a.exec();
}
