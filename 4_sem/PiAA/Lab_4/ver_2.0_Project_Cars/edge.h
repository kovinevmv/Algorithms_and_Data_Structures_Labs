#ifndef EDGE_H
#define EDGE_H
#include <QGraphicsItem>
#include <QtWidgets>

class Node;
class MScene;

//Класс для описания рёбер графа
class Edge : public QObject, public QGraphicsItem
{
	Q_OBJECT	//для реализация слотов и сигналов
	Q_INTERFACES(QGraphicsItem)
public:
	Edge(Node *sourceNode, Node *destNode, MScene *sc, int val = 1,QObject * obj = Q_NULLPTR);
//Конструктор принимает вершину родителя, вершину ребенка, вес ребра (пропускную способность)

	~Edge();
	void setColor(QColor col); //установить цвет ребра
	void setStyle(Qt::PenStyle st = Qt::SolidLine); //установить стиль ребра

	void setValue(int n); //установить пропускную способность
	int getValue() const; //пропускная способность

	Node *sourceNode() const; //вершина источник
	Node *destNode() const;	  //вершина ребенок

	void adjust();//Перерисовка линий при перетасквании Node

	bool setCurrent(int n); //установить текущий поток через данное ребро
	int getCurrent() const; //текущий поток через данное ребро

	void setAnimating(bool a); //a - отображать ли анимацию
	bool isAnimating() const; //производится ли на ребре анимация

	void remove(); //удалить ребро

	enum { Type = UserType + 2 }; //Необходимо для определения типа объекта
	int type() const Q_DECL_OVERRIDE; //тип для определения графического объекта

protected:
	QRectF boundingRect() const Q_DECL_OVERRIDE; //ограничивающий прямоугольник
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

	QPainterPath shape() const Q_DECL_OVERRIDE; //форма объекта
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event)Q_DECL_OVERRIDE; //контекстное меню
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE; //обработка двойного нажатия
private:
	Node	*source, *dest; //Соединяемые вершины

	QPointF sourcePoint; //Координаты концов линии
	QPointF destPoint;

	MScene* scene;

	qreal			arrowSize;   //Размер стрелки
	int				node_diametr; //Диаметр вершины
	QColor			main_color; //цвет
	Qt::PenStyle	style; //стиль


	int		value;   //пропускная способность
	int		currentValue; //текущий поток

	bool shouldShowCurrent; //отображать ли текущий поток

	QGraphicsPixmapItem		*it; //объект на ребре
	QGraphicsItemAnimation	*posAnim; //анимация объекта it
	QTimeLine				*timer; //таймер для анимации
	bool					animating;  //анимируется ли ребро

	QGraphicsPixmapItem		*dop_it;
	QGraphicsItemAnimation	*dop_posAnim;
	QTimeLine				*dop_timer;

	void animate();  //анимировать ребро
	void deleteAnimation(); //удалить анимацию
private slots:

	void updateAnimation(); //обновить координаты анимации при перетаскивании вершин

	void popupAnim(); //Анимация всплывающего текста над вершиной-приемником

signals:

	void positionChanged(); //сигнал о том, что изменилась геометрия

};

#endif // EDGE_H
