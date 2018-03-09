#include "graphwidget.h"
#include "edge.h"
#include "node.h"
#include <math.h>
#include <QRandomGenerator>
#include <iostream>
#include <vector>
#include <QPointF>
using namespace std;

GraphWidget::GraphWidget(vector<vector<MyNode>> a, QTextEdit* textEdit, int flag, QWidget *parent)
    : QGraphicsView(parent), timerId(0)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(0, 0, 600, 600);
    setScene(scene);

    vector<Node*> array(a[0].size());
    for (unsigned int i=0; i<array.size(); i++)
    {

        array[i] = new Node(a[i][i].name, textEdit, a[i][i].pos, this);
        array[i]->setPos(a[i][i].pos);
        scene->addItem(array[i]);
    }

    for (unsigned int i=0; i < a[0].size(); i++)
    {
        for (unsigned int j=0; j< a[0].size(); j++)
        {
            if (a[i][j].value != 0)
            {
              scene->addItem(new Edge(array[j], array[i], flag));
            }
        }
    }

}

void GraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}

void GraphWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    QList<Node*> nodes;
    foreach (QGraphicsItem *item, scene()->items())
    {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;

    }
}

