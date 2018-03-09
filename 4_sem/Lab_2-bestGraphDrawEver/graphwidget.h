#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QTextEdit>
#include <algorithm>
#include <QPushButton>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMainWindow>
#include <vector>
#include <map>
using namespace std;

class Node;

struct MyNode
{
    QString name;
    int value;
    QPointF pos;
};


class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(vector<vector<MyNode>>, QTextEdit* , int, QWidget *parent = 0);
    void itemMoved();

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    int timerId;

};

#endif // GRAPHWIDGET_H
