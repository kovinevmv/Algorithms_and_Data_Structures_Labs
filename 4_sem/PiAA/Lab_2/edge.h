#pragma once
#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>
#include <qmath.h>
#include <QPainter>

class Node;

class Edge : public QGraphicsItem
{
public:
    Edge(Node *sourceNode, Node *destNode, int direction, QColor vv);

    Node *sourceNode() const;
    Node *destNode() const;
    int direction;
    void adjust();
    enum { Type = UserType + 2 };
    int type() const override { return Type; }

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    Node *source, *dest;
    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;
    QColor color;
};

#endif // EDGE_H
