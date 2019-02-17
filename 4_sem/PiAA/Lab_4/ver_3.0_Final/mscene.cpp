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
    // Очистить контейнеры с удаленными объектами
	foreach(QGraphicsItem* node,deleted_it)
		delete node;
}

Edge* MScene::findEdge(Node *source, Node *dest)
{
    // Поиск ребра по заданным вершинам
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
        do		// Цикл для вычисления уникального имени новой вершины
			value = QString::number(i++);
		while(findNode(value));

	}
    Node * n = new Node(this,value); // Создать вершину
	addItem(n);
	n->setPos(position);
    all_nodes << n; // Добавить в список вершин
	performed_actions << MScene::smth_added;
	return n;
}

Edge *MScene::addEdge(Node *source, Node *dest,int val)
{
    if(Node::isNodesConnected(source,dest))
		return Q_NULLPTR;
    // Если уже соединены - остановить

	Edge * e = new Edge(source,dest,this, val);
	addItem(e);
	performed_actions << MScene::smth_added;
	return e;

}

void MScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseDoubleClickEvent(event);

    // Определить, находится ли под мышью какой-либо объект
	QGraphicsItem* temp = itemAt(event->scenePos().x(),event->scenePos().y(), QTransform::fromScale(1,1) );

    // Если объекта нет - добавить вершину
	if(event->button() == Qt::LeftButton && temp == Q_NULLPTR )
			addNode(event->scenePos());
}

void MScene::connectTwoNodes()
{
    // Если выбран один элемент и этот элемент вершина - присвоить first эту вершину
	if(selectedItems().size() == 1 && selectedItems().last()->type() == Node::Type)
		first = qgraphicsitem_cast<Node*>(selectedItems().first());

    // Если выделено 2 элемента
	if(selectedItems().size() == 2 && QApplication::keyboardModifiers() == Qt::ShiftModifier )
	{
        // Присваиваем второму элементу оставшийся выделенный
		second = (first==selectedItems().last()) ? selectedItems().first() : selectedItems().last();


		if(second->type() != Node::Type || first->type() != Node::Type)
		{
            // Если тип второго элемента не вершина - выйти
			clearSelection();
			return;
		}

        if (Node::isNodesConnected(first,(Node*)second))
		{
            // Если вершины уже соединены - выйти
			clearSelection();
			second->setSelected(true);
			return;
		}
        // Создать ребро
		addEdge(first,(Node*)second);
		clearSelection();
		second->setSelected(true);
	}
}

void MScene::setColorOfSelectedNodes()
{
    // Получение цвета специальным диалоговым окном
    QColor color = QColorDialog::getColor(QColor(255,235,0),parent,QString("Цвет вершины"));
	if(!color.isValid())
		return;
	foreach (QGraphicsItem* node, selectedItems())
	{
        // Окрасить все вершины
		if(node->type() == Node::Type)
			((Node*)node)->setColor(color);
	}
    clearSelection(); // Снять выделение
}

void MScene::removeSelectedNodes()
{
    // Сначала удаляются все выделенные ребра
	foreach (QGraphicsItem* edge, selectedItems())
		if(edge->type() == Edge::Type)
			((Edge*)edge)->remove();

    // Далее удаляются все выделенные вершины (и связанные с ними невыделенные ребра)
	foreach (QGraphicsItem* node, selectedItems())
		if(node->type() == Node::Type)
			((Node*)node)->remove();
}

// Создание графа из списка инцидентности (расположение автоматически по кругу)
void MScene::createGraphWithText(QList<QString> &nodes, QList<QStringList> &child_of_nodes)
{

    foreach (QGraphicsItem* node, deleted_it)
        if(node->type() == Node::Type)
            ((Node*)node)->remove();

	deleted_it.clear();
	performed_actions.clear();

    QList<QString> all_n; // Все вершины
	for (int i = 0; i < nodes.size();i++)
		if(!all_n.contains(nodes[i]))
				all_n << nodes[i];
	for (int i = 0; i < nodes.size();i++)
		for(int k = 0; k < child_of_nodes[i].size();k++)
			if(!all_n.contains(child_of_nodes[i][k]))
				all_n << child_of_nodes[i][k];


	int radius = 200;
    double segments = (TwoPi)/all_n.size(); // Вычисление радиуса для расстановки по кругу
	if (all_n.size() >= 16)
		radius = all_n.size()*70/6;
	double current_angle = 0;

	foreach(QString str,all_n)
	{
        // Создание всех вершин и размещение их по кругу
		QPointF pos(radius*cos(current_angle),radius*sin(current_angle));
		addNode(pos,str);
		current_angle += segments;
	}

	for (int i = 0; i < nodes.size(); i++)
	{
        Node * a = findNode(nodes[i]); // Ищем элемент
		for(int k = 0; k < child_of_nodes[i].size(); k++)
		{
            // Ищем детей a
			Node* b = findNode(child_of_nodes[i][k]);

            if (a && b) // Елси найдены
			{
                if (!Node::isNodesConnected(a,b)) // Не соединены - добавить соединение
					addEdge(a, b);
			}
			else
				continue;
		}
	}
}

Node* MScene::findNode(QString val)
{
    // Поиск вершины по значению
	foreach (Node* node, all_nodes)
		if(node->getValue() == val)
			return node;
	return Q_NULLPTR;
}

void MScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Для отмены снятия выделение при зажатом шифте
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
        copy_n << startNode; // Первым заносим стартовую вершину,
                             // относительно которой будет вставка

    // Заполнение копируемых вершин
	foreach (QGraphicsItem* it, selectedItems()) {
		if(it->type() == Node::Type)
		{
			if(it != startNode)
				copy_n << qgraphicsitem_cast<Node*>(it);
		}
	}

    // Заполнение копируемых ребер
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

// Контекстное меню сцены
void MScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	QGraphicsScene::contextMenuEvent(event);
	QGraphicsItem * it =itemAt(event->scenePos(), QTransform::fromScale(1,1) );

    if(it == Q_NULLPTR) // Меню вызвается если под курсором нет объекта
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

    QPointF offset = p - copy_n.first()->scenePos(); // Вычисляем смещение

	QList<Node*> temp_n;
	foreach (Node* node, copy_n) {
        temp_n << addNode(node->scenePos() + offset); // Добавляем вершины со смещением
		temp_n.last()->setColor(node->getColor());
	}

	foreach (Edge* ed, copy_e) {
        int a = copy_n.indexOf(ed->sourceNode()); // Добавляем связи
        int b = copy_n.indexOf(ed->destNode());	  // у новых вершин
		addEdge(temp_n.at(a),temp_n.at(b),ed->getValue());
	}

    foreach (Node* n, temp_n) //Выделяем вставленное
		n->setSelected(true);
}
