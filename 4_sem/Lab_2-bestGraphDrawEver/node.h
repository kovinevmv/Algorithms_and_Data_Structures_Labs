
#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>
#include <QTextEdit>


class Edge;
class GraphWidget;
class QGraphicsSceneMouseEvent;




class Node : public QGraphicsItem
{
public:
    Node(QString, QTextEdit* , QPointF,  GraphWidget *graphWidget);

    void addEdge(Edge *edge);
    QList<Edge *> edges() const;

    enum { Type = UserType + 1 };
    int type() const override { return Type; }

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QString maintext;
    QTextEdit* textEdit;


protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QList<Edge *> edgeList;
    GraphWidget *graph;
    QPointF position;
};

#endif // NODE_H
