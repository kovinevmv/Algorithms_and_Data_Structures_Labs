#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include <QtCore>
#include <QtGui>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void conv(QString l, int **L, int N);
    void pnt(int **F, int N, int*);
    void obr(float x1, float y1, float x2, float y2, int ves);
    void pathfinder(int **F, int N, int **next, int **d);
    void gen(int **F,int N);
    ~MainWindow();

private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_action_2_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsEllipseItem *ellipse;
    QGraphicsLineItem *line;
    QGraphicsTextItem *txt;
    QGraphicsTextItem *txt1;
    QTimer *timer;
};

#endif // MAINWINDOW_H
