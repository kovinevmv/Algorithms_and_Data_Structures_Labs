#include "mainwindow.h"
#include <QApplication>
#include <QTime>
#include <QStyle>
#include <QFile>
#include <QtWidgets>

void animation(QSplashScreen* splash)
{
    QLabel splashlabel(splash);
    QMovie splashgif(":icons/3.gif");
    splashlabel.setMovie(&splashgif);
    splashgif.start();
    splash->show();
    splash->setCursor(Qt::BlankCursor);
    for(int i=0;i<30000;i+=splashgif.speed())
    {
        QCoreApplication::processEvents();
        Sleep(splashgif.speed()/5);

    }

}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setQuitOnLastWindowClosed(true);

    QPixmap pix(":icons/3.gif");
    QSplashScreen splash(pix);
    animation(&splash);

    GraphWindow* w = new GraphWindow;
    w->show();
    splash.finish(w);

	return a.exec();
}
