#include "graphwidget.h"
#include <QtWidgets>


// Конструктор Node
Node::Node(QString text, QTextEdit* textEdit1, QPointF pos, QColor cur_color, GraphWidget *graphWidget)
    : graph(graphWidget)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    maintext = text;
    textEdit = textEdit1;
    position = pos;
    color = cur_color;
}


// Добавление ребра
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
    // Отрисовка цвета вершины
    painter->setRenderHint(QPainter::Antialiasing);
    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken)
        gradient.setColorAt(0, QColor(QColor(color.red()/1.7, color.green()/1.7, color.blue()/1.7)).light(120));
    else
        gradient.setColorAt(0, color);

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


void Node::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{

    // Меню по правому клику
    QMenu * menu = new QMenu;
    QAction * je = menu->addAction("Переименовать");
    QAction * se = menu->exec(event->screenPos());
    if(se==je)
    {
        QString a;
        bool ok;
        a = QInputDialog::getText(graph,"Вершина","Новая имя",QLineEdit::Normal,maintext,&ok);
        if(ok && !a.isEmpty())
            maintext = a;
        update();

    }

   QGraphicsItem::contextMenuEvent(event);
}


