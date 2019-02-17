#include <QtWidgets>

#include "node.h"
#include "mscene.h"
#include "mgraphview.h"
#include "edge.h"

Node::Node(MScene *graphWidget, QString val)
	: scene(graphWidget), value(val)
{
    // Флаги, что объект перетаскиватся, выделяется и отправляет изменения
	setFlags(ItemIsMovable |  ItemSendsScenePositionChanges|ItemIsSelectable);
	setCacheMode(DeviceCoordinateCache);
	setZValue(1);
}

// Установить форму объекта - окружность
QPainterPath Node::shape() const
{
	QPainterPath path;
	path.addEllipse(-diametr/2, -diametr/2, diametr, diametr);
	return path;
}

// Установаить ограничивающую область фигуры для захвата курсором
QRectF Node::boundingRect() const
{
	qreal adjust = 20;
	return QRectF( -diametr/2 - adjust, -diametr/2 - adjust, diametr+ adjust*2, diametr + adjust*2);
}

// Окраска вершины
void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
	Q_UNUSED(option);

    QRadialGradient gradient(-10, -10, diametr/2);

    // Толщина линии в зависимости от выделения
    if(isSelected())
		painter->setPen(QPen(Qt::black,4));
	else
		painter->setPen(QPen(Qt::black, 2));

    // Установить цвет градиента
    gradient.setColorAt(0, main_color);
    gradient.setColorAt(1, main_color.darker(150));

    // Заливка фигуры и добавление текста (имени вершины)
	painter->setBrush(gradient);
	painter->drawEllipse(-diametr/2, -diametr/2, diametr, diametr);
	painter->drawText(-diametr/2-20, -diametr/2, diametr+40, diametr,Qt::AlignCenter,value);
	update();
}

// Именение положения вершины
QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    // Проверка на изменение положения вершины
     if(change == ItemScenePositionHasChanged)
	 {
         // Если изменено - пересчитать геометрию всех ребер
        foreach (Edge *edge, edgeList)
			edge->adjust();
	 }

	return QGraphicsItem::itemChange(change, value);
}

// Соединены ли вершины
bool Node::isNodesConnected(Node* sourceNode, Node* destinationNode)
{
    if(sourceNode->childrenList.contains(destinationNode))
		return true;
    else
        return false;
}

 // Установка контексного меню при правом клике мышью
void Node::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    // Установка контексного меню при правом клике мышью
	QGraphicsItem::contextMenuEvent(event);
	setSelected(true);


    // Меню вариантов действий с вершиной
	QMenu menu;
        QAction *renameNodeName_state  = menu.addAction("Переименовать");
        QAction *changeColorNode_state = menu.addAction("Изменить цвет");
	menu.addSeparator();
        QAction *ist = menu.addAction("Установить как исток");
        QAction *stok = menu.addAction("Установить как сток");
    menu.addSeparator();
        QAction *removeNode_state  = menu.addAction("Удалить вершину");
        QAction *copy = menu.addAction("Копировать");

    QAction *selectedState = menu.exec(event->screenPos());

    // Выполнение действий
    if(selectedState == changeColorNode_state)
		scene->setColorOfSelectedNodes();    
    else if(selectedState == renameNodeName_state)
	{
        bool correctRename;
        QString text = QInputDialog::getText(scene->getMGraph(),
               "Переименовать", "Имя вершины:", QLineEdit::Normal, value,&correctRename);
        if(correctRename && !text.isEmpty())
			setValue(text);
	}
    else if(selectedState == removeNode_state)
        scene->removeSelectedNodes();
    else if(selectedState == ist)
		setValue("S");
    else if(selectedState == stok)
		setValue("T");
    else if (selectedState == copy)
        scene->copySelectedNodes();

	setSelected(false);

}

// Обработка двойного клика на вершину
void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);

    // Вызов меню переименования имени вершины
    bool correctRename;
    QString text = QInputDialog::getText(scene->getMGraph(),
             "Переименовать", "Имя вершины:", QLineEdit::Normal, value,&correctRename);
    if(correctRename && !text.isEmpty())
        setValue(text);
}

// Удаление вершины
void Node::remove()
{    
    // Удалить все инцидентные ребра
    foreach (Edge* edge, edgeList)
    {
		edge->remove();
		edge->sourceNode()->update();
		edge->destNode()->update();
	}
    scene->nodesList()->removeAll(this);
	scene->removeItem(this);

    // Записать в контейнер удаленных вершин
    scene->deleted_it << this;
	scene->performed_actions << MScene::smth_delted;
}

// Оператор << для вершины
QString& operator<<(QString & str, const Node & node)
{
	str.append(node.value + " : ");
    if(node.childrenList.isEmpty())
		return str;

    foreach (Node* n, node.childrenList)
    {
        if (n == node.childrenList.last())
            str.append(n->value);
        else
            str.append(n->value + ", ");
    }

	return str;

}

// Добавление ребра в список ребер
void Node::addEdge(Edge *edge)
{
    edgeList << edge;
}

// Добавление наследника
void Node::addChild(Node * child)
{
    childrenList << child;
}

// Функиця, возвращающая список вершин
QList<Edge*>* Node::getEdges()
{
    return &edgeList;
}

// Функция, возвращащающая список наследников
QList<Node*>* Node::getChildren()
{
    return &childrenList;
}

// Установка цвета вершины
void Node::setColor(QColor _color)
{
    main_color = _color;
	update();
}

// Установка имени вершины
void Node::setValue(QString val)
{
    value = val;
    update();
}

// Переопредление типа
int Node::type() const
{
    return Type;
}

// Функиця, возвращающая имя вершины
QString Node::getValue() const
{
    return value;
}

// Функиця, возвращающая цвет вершины
QColor Node::getColor() const
{
    return main_color;
}
