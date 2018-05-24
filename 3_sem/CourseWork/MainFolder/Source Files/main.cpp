#include "mainwindow.h"
#include <QApplication>
#include <time.h>

int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication a(argc, argv);
    MainWindow w;


    w.setFixedSize(340,435);
    w.setWindowTitle("Курсовая работа");
    w.setWindowIcon(QIcon("D:/2.png"));
    w.show();


    return a.exec();
}
