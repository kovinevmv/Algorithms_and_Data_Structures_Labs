#include <QtWidgets>

#include "node.h"
#include "mscene.h"
#include "mgraphview.h"
#include "edge.h"

Node::Node(MScene *graphWidget, QString val)
	: scene(graphWidget), value(val)
{
//флаги, что объект перетаскиватся, выделяется и отправляет изменения
	setFlags(ItemIsMovable |  ItemSendsScenePositionChanges|ItemIsSelectable);
	setCacheMode(DeviceCoordinateCache);
	setZValue(1);
	//scene->performed_actions << MScene::smth_added;
}

QPainterPath Node::shape() const //форма объекта - окружность
{

	QPainterPath path;
	path.addEllipse(-diametr/2, -diametr/2, diametr, diametr);
	return path;
}

QRectF Node::boundingRect() const
{
	qreal adjust = 20;
	return QRectF( -diametr/2 - adjust, -diametr/2 - adjust, diametr+ adjust*2, diametr + adjust*2);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
	Q_UNUSED(option);

    QRadialGradient gradient(-10, -10, diametr/2);

	if(isSelected()) //толщина линии в зависимости от выделения
		painter->setPen(QPen(Qt::black,4));
	else
		painter->setPen(QPen(Qt::black, 2));


	gradient.setColorAt(0, main_color); //установить цвет градиента

	painter->setBrush(gradient);
	painter->drawEllipse(-diametr/2, -diametr/2, diametr, diametr);
	painter->drawText(-diametr/2-20, -diametr/2, diametr+40, diametr,Qt::AlignCenter,value);
	update();
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
	 if(change==ItemScenePositionHasChanged)
	 {
		 //если изменено положение вершины - пересчитать геометрию всех ребер
		foreach (Edge *edge, edgeList)
			edge->adjust();
	 }
	return QGraphicsItem::itemChange(change, value);
}

bool Node::is_two_nodes_connected(Node* src, Node* dst)
{
	if(src->childrenList.contains(dst))
		return true;
	return false;
}

void Node::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	QGraphicsItem::contextMenuEvent(event);
	setSelected(true);

	QMenu menu;
		QAction *rename_action = menu.addAction("Переименовать");
		QAction *color_change_action = menu.addAction("Изменить цвет");
	menu.addSeparator();
	QAction *ist = menu.addAction("Установить как исток");
	QAction *stok = menu.addAction("Установить как сток");
	menu.addSeparator();
		QAction *self_in_action = menu.addAction("Создать петлю");


		QAction *remove_action = menu.addAction("Удалить вершину");
		QAction *selected_action = menu.exec(event->screenPos());

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
	else if(selected_action == self_in_action)
		setLoop();
	else if(selected_action == ist)
		setValue("S");
	else if(selected_action == stok)
		setValue("T");

	setSelected(false);

}

void Node::remove()
{
	foreach (Edge* edge, edgeList) {
		edge->remove();
		edge->sourceNode()->update();
		edge->destNode()->update();
	}
	scene->nodesList()->removeAll(this);
	scene->removeItem(this);
	scene->deleted_it << this; //запсать в контейнер удаленных вершин
	scene->performed_actions << MScene::smth_delted;
}

QString& operator<<(QString & str, const Node & node)
{
	str.append(node.value + " : ");
	if(node.childrenList.isEmpty())
		return str;

	foreach (Node* n, node.childrenList)
		if (n == node.childrenList.last())
			str.append(n->value);
		else
			str.append(n->value + ", ");
	return str;

}

void Node::setLoop()
{
	//при создании петли не рисуется ребро - просто в дети добавляется сама вершина
	if(!childrenList.contains(this))
		childrenList << this;
	update();
}

void Node::addEdge(Edge *edge){edgeList << edge;}

void Node::addChild(Node * child){childrenList << child;}

QList<Edge *> * Node::edges(){return &edgeList;}

QList<Node *> * Node::children(){return &childrenList;}

void Node::setColor(QColor col)
{
    main_color = col;
	update();

}

int Node::type() const{ return Type; }
QString Node::getValue()const {return value;}
QColor Node::getColor() const{return main_color;}


void Node::setValue(QString val){value = val;update();}//установить значение

void Node::removeLoop()
{
	if(childrenList.contains(this))
		childrenList.removeAll(this);
	update();
}
void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

	QGraphicsItem::mouseDoubleClickEvent(event);
	bool ok;
	QString text = QInputDialog::getText(scene->getMGraph(), "Переименовать",
												 "Имя вершины:", QLineEdit::Normal,
												 value,&ok);
	if(ok && !text.isEmpty())
		setValue(text);
}
