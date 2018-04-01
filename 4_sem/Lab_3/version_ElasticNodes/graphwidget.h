#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include "edge.h"
#include "node.h"
#include "biconnected.h"
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
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <math.h>
#include <iostream>
#include <QPointF>
#include "helpbrowser.h"
using namespace std;

class Node;

struct MyNode
{
    QString name;
    int value;
    QPointF pos;
    QColor vv;
    QColor vv2;
};


class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(vector<vector<MyNode>>, QTextEdit* , int,  QWidget *parent = 0);
    void itemMoved();

protected:
    void timerEvent(QTimerEvent *event) override;


    void scaleView(qreal scaleFactor);


private:
    int timerId;

};

#endif // GRAPHWIDGET_H
