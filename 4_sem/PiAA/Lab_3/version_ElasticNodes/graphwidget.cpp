#include "graphwidget.h"
#include "edge.h"
#include "node.h"

GraphWidget::GraphWidget(vector<vector<MyNode>> a, QTextEdit* textEdit, int flag, QWidget *parent)
    : QGraphicsView(parent), timerId(0)
{
    setCacheMode(CacheBackground);
        setViewportUpdateMode(BoundingRectViewportUpdate);
        setRenderHint(QPainter::Antialiasing);
        setTransformationAnchor(AnchorUnderMouse);
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(0, 0, 600, 600);

    setScene(scene);

    vector<Node*> array(a[0].size());
    for (unsigned int i=0; i<array.size(); i++)
    {
        array[i] = new Node(a[i][i].name, textEdit, a[i][i].pos, a[i][i].vv, this);
        array[i]->setPos(a[i][i].pos);
        array[i]->setFlag(QGraphicsItem::ItemIsSelectable);
        scene->addItem(array[i]);
    }

    for (unsigned int i=0; i < a[0].size(); i++)
    {
        for (unsigned int j=0; j< a[0].size(); j++)
        {
            if (a[i][j].value != 0)
            {
              scene->addItem(new Edge(array[j], array[i], flag, a[i][j].vv2, a[i][j].value));
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



void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}
