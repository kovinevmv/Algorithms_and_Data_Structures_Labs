
#include "edge.h"
#include "node.h"
#include "graphwidget.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <fstream>
#include <QDebug>
using namespace std;



Node::Node(QString text, QTextEdit* textEdit1, QPointF pos, GraphWidget *graphWidget)
    : graph(graphWidget)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);

    maintext = text;
    textEdit = textEdit1;
    position = pos;
}

void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

QList<Edge *> Node::edges() const
{
    return edgeList;
}


QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -20 - adjust, -20 - adjust, 230 + adjust, 230 + adjust);
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-20, -20, 40, 40);
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{

    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken)
        gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
    else
        gradient.setColorAt(0, Qt::yellow);

    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-20, -20, 40, 40);

    QRectF textRect(-5,-7, 1000, 1000);
    painter->drawText(textRect, maintext);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change)
    {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList)
            edge->adjust();
        graph->itemMoved();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

    textEdit->setText(textEdit->toPlainText() + maintext + " ");

    update();
    QGraphicsItem::mouseDoubleClickEvent(event);
};
