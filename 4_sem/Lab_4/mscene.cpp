#include <QtWidgets>
#include <QDialog>
#include "mscene.h"
#include "node.h"
#include "edge.h"
#include "mgraphview.h"

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

MScene::MScene(QObject *par)
    : QGraphicsScene(par)
    , show_edge_values(true)
    , graph_oriented(true)
    , node_diametr(43)
    , arrow_size(13)
{
    parent =  (MGraphView*)par;

    setItemIndexMethod(QGraphicsScene::NoIndex);
    connect(this,&MScene::selectionChanged,this,&MScene::connectTwoNodes);
}

int MScene::diametr() const
{
    return node_diametr;
}

QList<Node*> * MScene::nodesList()
{
    return &all_nodes;
}

MGraphView * MScene::getMGraph()
{
    return parent;
}

MScene::~MScene()
{
    foreach (QGraphicsItem * it, deleted) {
        delete it;
    }
    qDebug() << "scene deleted";
}

void MScene::setDiametr(int d)
{
    node_diametr = d;
}

void MScene::setArrowSize(int d)
{
    arrow_size = d;
}

bool MScene::isOriented() const
{
    return graph_oriented;
}

//установить ориентированность графа
void MScene::setOrientated(bool b)
{
    graph_oriented = b;
    foreach (QGraphicsItem* it, items()) {
        if(it->type() == Edge::Type)
            ((Edge*)it)->setOneArrowAtEdge(b);
    }
}

//добавить вершину
void MScene::addNode(QPointF position, QString value)
{
    if(value.isEmpty())
    {
        int i = 0;
        do
            value = QString::number(i++);               //определение автоматического имени
        while(findNode(value));
    }

    Node * n = new Node(this,value);
    addItem(n);
    n->setPos(position);
    all_nodes << n;
}

//добавить ребро
void MScene::addEdge(Node *source, Node *dest, int val)
{
    //если уже соединены - остановить
    if(Node::is_two_nodes_connected(source,dest))
        return;

    //если граф ориентированный
    if(graph_oriented)
    {
        if(Node::is_two_nodes_connected(dest,source))
        {
            //если уже соединено в обратном направлении
            //утстановить две стрелки и выйти
            foreach(Edge * ed,*dest->edges())
                if(ed->destNode() == source)
                {
                    ed->setOneArrowAtEdge(false);
                    return;
                }
        }

        addItem(new Edge(source,dest,this,val));            //добавить на сцену
    }
    else
    {
        //если неориентированный
        if(Node::is_two_nodes_connected(dest,source)
           || Node::is_two_nodes_connected(source,dest))
            return;                                         //если есть соединение - выйти

        Edge * ed = new Edge(source,dest,this,val);
        ed->setOneArrowAtEdge(false);                       //добавить на сцену и установить две стрелки
        addItem(ed);
    }

}

void MScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseDoubleClickEvent(event);
    QGraphicsItem* temp = itemAt(event->scenePos().x(),event->scenePos().y(), QTransform::fromScale(1,1) );
    if(event->button() == Qt::LeftButton && temp == Q_NULLPTR )
    {
        //если клик произведен не на вершине или ребре - добавить вершину
        addNode(event->scenePos());
    }

}

//соединение мышью
void MScene::connectTwoNodes()
{

    if(selectedItems().size() == 1 && selectedItems().last()->type() == Node::Type)
        first = (Node*)selectedItems().last();
    if(selectedItems().size() == 2  && QApplication::keyboardModifiers()== Qt::ShiftModifier)
    {
        second = (first==selectedItems().last()) ? selectedItems().first() : selectedItems().last();
        if(second->type() != Node::Type || first->type() != Node::Type)
        {
            clearSelection();
            return;
        }
        addEdge(first,(Node*)second);
        clearSelection();
        second->setSelected(true);
    }
}

void MScene::setShowEdge(bool a)
{
    show_edge_values = a;
    update();
}

bool MScene::shouldShowValues()const
{
    return show_edge_values;
}

//установить цвет выделенных вершин
void MScene::setColorOfSelectedNodes()
{
    QColor color = QColorDialog::getColor(Qt::white,parent,QString("Цвет вершины"));
    if(!color.isValid())
        return;

    foreach (QGraphicsItem* node, selectedItems())
    {
        if(node->type() == Node::Type)
            ((Node*)node)->setColor(color);
    }
    clearSelection();
}

//удалить выделенное
void MScene::removeSelectedNodes()
{
    foreach (QGraphicsItem* edge, selectedItems())
        if(edge->type() == Edge::Type)
            ((Edge*)edge)->removeEdge();


    foreach (QGraphicsItem* node, selectedItems())
        if(node->type() == Node::Type)
            ((Node*)node)->removeNode();
}

//создание графа
void MScene::createGraphWithText(QList<QString> &nodes, QList<QStringList> &child_of_nodes)
{
    foreach (QGraphicsItem* edge, selectedItems())
            if(edge->type() == Edge::Type)
                ((Edge*)edge)->removeEdge();


        foreach (QGraphicsItem* node, selectedItems())
            if(node->type() == Node::Type)
                ((Node*)node)->removeNode();

    deleted.clear();

    QList<QString> all_n;
    for (int i = 0; i < nodes.size();i++)
        if(!all_n.contains(nodes[i]))
                all_n << nodes[i];
    for (int i = 0; i < nodes.size();i++)
        for(int k = 0; k < child_of_nodes[i].size();k++)
            if(!all_n.contains(child_of_nodes[i][k]))
                all_n << child_of_nodes[i][k];


    int radius = 200;
    double segments = (TwoPi)/all_n.size();
    if (all_n.size() >= 16)
        radius = all_n.size()*70/6;
    double current_angle = 0;

    foreach(QString str,all_n)
    {
//авт. расчет точек по окружности
        QPointF pos(radius*cos(current_angle),radius*sin(current_angle));
        addNode(pos,str);
        current_angle += segments;
    }

//установка связей
    for (int i = 0; i < nodes.size(); i++){
        Node * a = findNode(nodes[i]);
        for(int k = 0; k < child_of_nodes[i].size(); k++)
        {
            Node* b = findNode(child_of_nodes[i][k]);
            if (a && b)
            {
                if (!Node::is_two_nodes_connected(a,b))
                    addEdge(a,b);
            }
            else
                continue;
        }
    }
}

//поиск вершины
Node* MScene::findNode(QString val)
{
    foreach (Node* node, all_nodes)
        if(node->get_val() == val)
            return node;
    return Q_NULLPTR;
}

//поиск ребра
Edge* MScene::findEdge(Node *source, Node *dest)
{
    foreach (Node* node, all_nodes)
    {
        if(node == source )
            foreach (Edge * edge, *node->edges())
                if(edge->destNode()==dest)
                    return edge;
    }
    return Q_NULLPTR;
}

void MScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem* temp = itemAt(event->scenePos().x(),event->scenePos().y(), QTransform::fromScale(1,1) );
    if(event->modifiers() == Qt::ShiftModifier && temp)
        temp->setSelected(true);
    QGraphicsScene::mousePressEvent(event);
}
