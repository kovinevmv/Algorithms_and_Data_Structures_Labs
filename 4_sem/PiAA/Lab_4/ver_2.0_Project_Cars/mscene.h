#ifndef MSCENE_H
#define MSCENE_H
#include <QtWidgets>

class GraphWindow;
class MGraphView;
class Node;
class Edge;
//Класс со сценой
class MScene : public QGraphicsScene
{
friend class GraphWindow;
friend class Edge;
friend class Node;
public:
	MScene(QObject* par = 0);
	~MScene();
	static void setDelay(int msec); //простая фукнция задержки на msec миллисекунд

	void setColorOfSelectedNodes(); //Установка цвета
	void removeSelectedNodes();

	void copySelectedNodes(Node* startNode = Q_NULLPTR); //скопировать выделенное
	void paste(QPointF p = QPoint(0,0)); //вставить элементы

	void undo(); //назад

//создать граф
	void createGraphWithText(QList<QString> & nodes, QList<QStringList> & child_of_nodes);

	QList<Node *> *nodesList(); //указатель на контейнер с вершинами

	Node* addNode(QPointF position, QString value = "");//добавить вершину
	Edge* addEdge(Node* source, Node* dest, int val = 1);

	Edge* findEdge(Node* source,Node* dest); //найти на сцене ребро
	Node* findNode(QString val); //найти на сцене вершину

	MGraphView * getMGraph() const; //виджет, в котором находится сцена

private:

	QList<Node*> all_nodes; //контейнер с текущими вершинами на сцене

	MGraphView *parent;  //виджет отображения данной сцены

	Node *first;    //указатели, необходимые для метода connectTwoNodes
	QGraphicsItem* second;

	//копирование
	QList<Node*> copy_n; //скопированные элементы
	QList<Edge*> copy_e;

	QList<QGraphicsItem*> deleted_it; //контейнер с удаленными вершинами

	enum action {smth_delted,smth_added};
	QVector<action> performed_actions;
protected:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event) Q_DECL_OVERRIDE;
	void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) Q_DECL_OVERRIDE;

private slots:
	void connectTwoNodes();
//соединить две вершины с помощью мыши


};

#endif // MSCENE_H
