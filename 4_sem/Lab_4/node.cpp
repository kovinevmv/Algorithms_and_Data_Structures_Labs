#include <QtWidgets>

#include "node.h"
#include "mscene.h"
#include "mgraphview.h"
#include "edge.h"

Node::Node(MScene *graphWidget, QString val)
    : scene(graphWidget)
    , main_color(QColor(255, 223, 0))
    , value(val)
{
    setFlags(ItemIsMovable | ItemSendsGeometryChanges | ItemIsSelectable);
    setCacheMode(DeviceCoordinateCache);
    setAcceptHoverEvents(true);
}

//форма окружности
QPainterPath Node::shape() const
{
    QPainterPath path;
    int diametr = scene->diametr();
    path.addEllipse(-diametr/2, -diametr/2, diametr, diametr);
    return path;
}

QRectF Node::boundingRect() const
{
    qreal adjust = 20;
    int diametr = scene->diametr();
    return QRectF( -diametr/2 - adjust, -diametr/2 - adjust, diametr+ adjust*2, diametr + adjust*2);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{

    int diametr = scene->diametr();
    painter->setClipRect(option->exposedRect);
    if(childrenList.contains(this))
    {
        painter->setPen(QPen(Qt::black,2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawEllipse(-diametr*2/3,-diametr*2/3,diametr/2,diametr/2);
        painter->setBrush(Qt::black);
        painter->drawPolygon(QPolygonF() << QPointF(-17*diametr/30,-diametr/3) << QPointF(-7*diametr/15,-diametr/6)<< QPointF(-19*diametr/30,-diametr/6));
    }

    QRadialGradient gradient(-10, -10, diametr/2);
    if(isSelected())
        painter->setPen(QPen(Qt::black,3));
    else
        painter->setPen(QPen(Qt::black, 1));

    if(option->state.testFlag(QStyle::State_MouseOver))
    {

        gradient.setColorAt(1, QColor(255,153,0));
    }
    else
    {
        gradient.setColorAt(0, main_color);

    }
    painter->setBrush(gradient);
    painter->drawEllipse(-diametr/2, -diametr/2, diametr, diametr);
    QTextOption opt(Qt::AlignCenter);
    opt.setWrapMode(QTextOption::WrapAnywhere);
    painter->drawText(QRectF(-diametr/2+10, -diametr/2, diametr-20, diametr),value,opt);
    update();
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
     if(change==ItemPositionHasChanged)
     {
        foreach (Edge *edge, edgeList)
            edge->adjust();
     }

    return QGraphicsItem::itemChange(change, value);
}

//соединены ли две вершины
bool Node::is_two_nodes_connected(Node* src, Node* dst)
{
    if(src->childrenList.contains(dst))
        return true;
    return false;
}

//контекстное меню
void Node::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QGraphicsItem::contextMenuEvent(event);
    setSelected(true);

    QMenu menu;
    QAction * rename_action = menu.addAction("Переименовать");
    QAction * color_change_action = menu.addAction("Изменить цвет");
    menu.addSeparator();
    QAction * remove_action = menu.addAction("Удалить");

    QAction * selected_action = menu.exec(event->screenPos());

    if(selected_action == color_change_action)
        scene->setColorOfSelectedNodes();
    else if(selected_action == rename_action)
    {
        bool ok;
        QString text = QInputDialog::getText(scene->getMGraph(), "Переименовать",
                                                     "Имя вершины:", QLineEdit::Normal,
                                                     value,&ok);
        if(ok && !text.isEmpty())
            setValue(text);
    }
    else if(selected_action == remove_action)
        scene->removeSelectedNodes();
    setSelected(false);
}

void Node::removeNode()
{
    foreach (Edge* edge, edgeList) {
        edge->removeEdge();
        edge->sourceNode()->update();
        edge->destNode()->update();
    }
    scene->nodesList()->removeAll(this);
    scene->removeItem(this);
    scene->deleted << this;

}

QString& operator<<(QString & str, const Node & node)
{
    str.append(node.value + " : ");
    if(node.childrenList.isEmpty())
    {
        return str;
    }
    foreach (Node* n, node.childrenList)
        if (n == node.childrenList.last())
            str.append(n->value);
        else
            str.append(n->value + ", ");
    return str;

}

void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

void Node::addChild(Node * child)
{
    childrenList << child;
}

QList<Edge *> * Node::edges()
{
    return &edgeList;
}

QList<Node *> * Node::children()
{
    return &childrenList;
}

void Node::setColor(QColor col)
{
    if( col == Qt::black)
        return;

    main_color = col;
    update();
}

int Node::type() const
{
    return Type;
}

QString Node::get_val()const
{
    return value;
}

QColor Node::get_color() const
{
    return main_color;
}

//установить значение
void Node::setValue(QString val)
{
    value = val;update();
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

    QGraphicsItem::mouseDoubleClickEvent(event);
    if(event->button()!= Qt::LeftButton)
        return;
    bool ok;
    QString text = QInputDialog::getText(scene->getMGraph(), "Переименовать",
                                                 "Имя вершины:", QLineEdit::Normal,
                                                 value,&ok);
    if(ok && !text.isEmpty())
        setValue(text);
}
