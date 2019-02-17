#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>

class Edge;
class MGraphView;
class MScene;

//Класс для определения вершин графа
class Node : public QGraphicsItem
{
public:
	Node(MScene *graphWidget,  QString val = "");

	~Node(){qDebug() <<"node deleted";}

	void setColor(QColor col);//установить цвет
	QColor getColor() const ;

	void setValue(QString val);//установить значение
	QString getValue() const;//извлечение значения

	void remove(); //удалить данную вершину
	void setLoop(); //создать петлю
	void removeLoop(); //удалить петлю

	QList<Edge *> * edges(); //список ребер
	QList<Node *> * children(); //список всех детей

	void addChild(Node * child); //Добавить ребенка к вершине
	void addEdge(Edge *edge); //Добавить ребро

	static bool is_two_nodes_connected(Node* src, Node* dst);
//Является ли src родителем dst

	enum { Type = UserType + 1 }; //Тип "вершина"
	int type() const Q_DECL_OVERRIDE; //определение типа

	friend QString& operator<<(QString & str, const Node & node);
//Оператор для записи в строчку

protected:
	QRectF boundingRect() const Q_DECL_OVERRIDE;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
	QPainterPath shape() const Q_DECL_OVERRIDE;
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event)Q_DECL_OVERRIDE;
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
private:
	QList<Edge *> edgeList; //Список ребер
	QList<Node *> childrenList; //Смежные вершины

	MScene * scene;   //сцена в которых находится вершина

    QColor main_color{QColor(255,235,0)}; //цвет
	int diametr{60};  //диаметр
	QString value;	 //значение
};


#endif // NODE_H
