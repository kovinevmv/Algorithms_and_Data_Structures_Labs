#include <QtWidgets>
#include <QDialog>

#include "mscene.h"
#include "node.h"
#include "edge.h"
#include "mgraphview.h"


static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;


// Конструктор класса MScene
MScene::MScene(QObject *par)
	:QGraphicsScene(par)
{
	parent =  (MGraphView*)par;
	setItemIndexMethod(QGraphicsScene::NoIndex);

    // При изменении выделения объектов вызвать метод для соединения
	connect(this,&QGraphicsScene::selectionChanged,this,&MScene::connectTwoNodes);
}


// Установка задержки
void MScene::setDelay(int msec)
{
    QTime dieTime= QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

QList<Node*>* MScene::nodesList()
{
    return &all_nodes;
}

MGraphView * MScene::getMGraph() const
{
    return parent;
}


MScene::~MScene()
{
	//очистить контейнеры с удаленными объектами
	foreach(QGraphicsItem* node,deleted_it)
		delete node;
	qDebug() << "mscene deleted";

}

Edge* MScene::findEdge(Node *source, Node *dest)
{
//поиск ребра по заданным вершинам
	foreach (Node* node, all_nodes)
	{
		if(node == source )
            foreach (Edge * edge, *node->getEdges())
				if(edge->destNode()==dest && edge->sourceNode() == source)
					return edge;

	}
	return Q_NULLPTR;
}
Node *MScene::addNode(QPointF position, QString value)
{
	if(value.isEmpty())
	{
		int i = 0;
		do		//цикл для вычисления уникального имени новой вершины
			value = QString::number(i++);
		while(findNode(value));

	}
	Node * n = new Node(this,value); //создать вершину
	addItem(n);
	n->setPos(position);
	all_nodes << n; //добавить в список вершин
	performed_actions << MScene::smth_added;
	return n;
}

Edge *MScene::addEdge(Node *source, Node *dest,int val)
{
    if(Node::isNodesConnected(source,dest))
		return Q_NULLPTR;
	//если уже соединены - остановить

	Edge * e = new Edge(source,dest,this, val);
	addItem(e);
	performed_actions << MScene::smth_added;
	return e;

}

void MScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseDoubleClickEvent(event);

	//определить, находится ли под мышью какой-либо объект
	QGraphicsItem* temp = itemAt(event->scenePos().x(),event->scenePos().y(), QTransform::fromScale(1,1) );

	//если объекта нет - добавить вершину
	if(event->button() == Qt::LeftButton && temp == Q_NULLPTR )
			addNode(event->scenePos());
}

void MScene::connectTwoNodes()
{
//если выбран один элемент и этот элемент вершина - присвоить first эту вершину
	if(selectedItems().size() == 1 && selectedItems().last()->type() == Node::Type)
		first = qgraphicsitem_cast<Node*>(selectedItems().first());

//если выделено 2 элемента
	if(selectedItems().size() == 2 && QApplication::keyboardModifiers() == Qt::ShiftModifier )
	{
		//присваиваем второму элементу оставшийся выделенный
		second = (first==selectedItems().last()) ? selectedItems().first() : selectedItems().last();


		if(second->type() != Node::Type || first->type() != Node::Type)
		{
			//если тип второго элемента не вершина - выйти

			clearSelection();
			return;
		}

        if (Node::isNodesConnected(first,(Node*)second))
		{
	//если вершины уже соединены - выйти
			clearSelection();
			second->setSelected(true);
			return;
		}
		//создать ребро
		addEdge(first,(Node*)second);

		clearSelection();
		second->setSelected(true);
	}
}

void MScene::setColorOfSelectedNodes()
{
	//получение цвета специальным диалоговым окном
    QColor color = QColorDialog::getColor(QColor(255,235,0),parent,QString("Цвет вершины"));
	if(!color.isValid())
		return;

	foreach (QGraphicsItem* node, selectedItems())
	{
		//окрасить все вершины
		if(node->type() == Node::Type)
			((Node*)node)->setColor(color);
	}
	clearSelection(); //снять выделение
}

void MScene::removeSelectedNodes()
{
	//сначала удаляются все выделенные ребра
	foreach (QGraphicsItem* edge, selectedItems())
		if(edge->type() == Edge::Type)
			((Edge*)edge)->remove();

	//далее удаляются все выделенные вершины (и связанные с ними невыделенные ребра)
	foreach (QGraphicsItem* node, selectedItems())
		if(node->type() == Node::Type)
			((Node*)node)->remove();
}

//создание графа из списка инцидентности (расположение автоматически по кругу)
void MScene::createGraphWithText(QList<QString> &nodes, QList<QStringList> &child_of_nodes)
{

    foreach (QGraphicsItem* node, deleted_it)
        if(node->type() == Node::Type)
            ((Node*)node)->remove();

	deleted_it.clear();
	performed_actions.clear();

	QList<QString> all_n; //все вершины
	for (int i = 0; i < nodes.size();i++)
		if(!all_n.contains(nodes[i]))
				all_n << nodes[i];
	for (int i = 0; i < nodes.size();i++)
		for(int k = 0; k < child_of_nodes[i].size();k++)
			if(!all_n.contains(child_of_nodes[i][k]))
				all_n << child_of_nodes[i][k];


	int radius = 200;
	double segments = (TwoPi)/all_n.size(); //вычисление радиуса для расстановки по кругу
	if (all_n.size() >= 16)
		radius = all_n.size()*70/6;
	double current_angle = 0;

	foreach(QString str,all_n)
	{
	//создание всех вершин и размещение их по кругу
		QPointF pos(radius*cos(current_angle),radius*sin(current_angle));
		addNode(pos,str);
		current_angle += segments;
	}

	for (int i = 0; i < nodes.size(); i++)
	{
		Node * a = findNode(nodes[i]); //ищем элемент
		for(int k = 0; k < child_of_nodes[i].size(); k++)
		{
			//ищем детей a
			Node* b = findNode(child_of_nodes[i][k]);

			if (a && b) //елси найдены
			{
                if (!Node::isNodesConnected(a,b)) //не соединены - добавить соединение
					addEdge(a, b);
			}
			else
				continue;
		}
	}
}

Node* MScene::findNode(QString val)
{
	//поиск вершины по значению
	foreach (Node* node, all_nodes)
		if(node->getValue() == val)
			return node;
	return Q_NULLPTR;
}

void MScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//для отмены снятия выделение при зажатом шифте
	QGraphicsItem* temp = itemAt(event->scenePos().x(),event->scenePos().y(), QTransform::fromScale(1,1) );
	if(event->modifiers() == Qt::ShiftModifier && temp)
		temp->setSelected(true);
	QGraphicsScene::mousePressEvent(event);
}


void MScene::copySelectedNodes(Node *startNode)
{
	copy_n.clear();
	copy_e.clear();

	if(startNode)
		copy_n << startNode; //первым заносим стартовую вершину,
							//относительно которой будет вставка

	//заполнение копируемых вершин
	foreach (QGraphicsItem* it, selectedItems()) {
		if(it->type() == Node::Type)
		{
			if(it != startNode)
				copy_n << qgraphicsitem_cast<Node*>(it);
		}
	}

	//заполнение копируемых ребер
	foreach (QGraphicsItem* it, selectedItems()) {
		if(it->type() == Edge::Type)
		{
			Edge * e = qgraphicsitem_cast<Edge*>(it);
			if(copy_n.contains(e->sourceNode()) && copy_n.contains(e->destNode()))
			{
				copy_e << e;
			}
		}
	}
	clearSelection();

}

//контекстное меню сцены
void MScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	QGraphicsScene::contextMenuEvent(event);
	QGraphicsItem * it =itemAt(event->scenePos(), QTransform::fromScale(1,1) );

	if(it == Q_NULLPTR) //меню вызвается если под курсором нет объекта
	{
		QMenu menu;
			QAction * paste_action = menu.addAction("Вставить");
		QAction * selected_action = menu.exec(event->screenPos());
		if(selected_action == paste_action)
			paste(event->scenePos());
	}

}


void MScene::paste(QPointF p)
{
	if(copy_n.isEmpty())
		return;
	clearSelection();

	QPointF offset = p - copy_n.first()->scenePos(); //вычисляем смещение

	QList<Node*> temp_n;
	foreach (Node* node, copy_n) {
		temp_n << addNode(node->scenePos() + offset); //добавляем вершины со смещением
		temp_n.last()->setColor(node->getColor());
	}

	foreach (Edge* ed, copy_e) {
		int a = copy_n.indexOf(ed->sourceNode()); //добавляем связи
		int b = copy_n.indexOf(ed->destNode());	//у новых вершин
		addEdge(temp_n.at(a),temp_n.at(b),ed->getValue());
	}

	foreach (Node* n, temp_n) //выделяем вставленное
		n->setSelected(true);

}

void MScene::undo()
{
	if(performed_actions.isEmpty())
		return;
	action act= performed_actions.last();
	performed_actions.pop_back();


	if(act == smth_delted)
	{
		QGraphicsItem* it = deleted_it.last();
		deleted_it.pop_back();
		if(it->type() == Node::Type)
		{
			addItem(it);
			all_nodes << (Node*)it;
		}
		if(it->type() == Edge::Type)
		{
			Edge * e = qgraphicsitem_cast<Edge *> (it);

			e->destNode()->addEdge(e);
			e->sourceNode()->addEdge(e);
			e->sourceNode()->addChild(e->destNode());

			addItem(e);
			e->adjust();

		}

	}
	if(act == smth_added)
	{
		QGraphicsItem* it = items().last();
		performed_actions << MScene::smth_added;
		if(it->type() == Node::Type)
			((Node*)it)->remove();
		if(it->type() == Edge::Type)
			((Edge*)it)->remove();
	}
}
