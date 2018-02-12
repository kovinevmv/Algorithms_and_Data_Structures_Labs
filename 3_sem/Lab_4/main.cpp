#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setWindowTitle("Лабораторная работа №4");
    w.setWindowIcon(QIcon("D:\\2.png"));
    w.setFixedSize(340, 340);
    w.show();

    return a.exec();
}
