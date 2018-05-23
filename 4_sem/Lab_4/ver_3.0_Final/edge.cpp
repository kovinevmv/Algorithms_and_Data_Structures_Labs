#include <math.h>
#include <QtWidgets>

#include "edge.h"
#include "node.h"
#include "mscene.h"
#include "mgraphview.h"

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

// Класс для описания ребер графа
Edge::Edge(Node *sourceNode, Node *destNode, MScene* sc, int val, QObject * obj)
    :QObject(obj), source(sourceNode), dest(destNode), scene(sc),
      arrowSize(15),node_diametr(60), main_color(Qt::black), style(Qt::SolidLine),
	  value(val),currentValue(0), shouldShowCurrent(false),animating(false)
{
    // Возможность выделять ребро
    setFlag(GraphicsItemFlag::ItemIsSelectable);
    setZValue(-1);

	source->addEdge(this);
	source->addChild(dest);
	dest->addEdge(this);

    // Создание таймера
    timer = new QTimeLine;
	timer->setFrameRange(0,500);
	timer->setLoopCount(1);

    // Создание анимации
    posAnim= new QGraphicsItemAnimation;
	posAnim->setTimeLine(timer);
	it = new QGraphicsPixmapItem;

    // Пересчет геометрии
    adjust();

    // При изменении геометрии пересчитать анимацию
	connect(this,&Edge::positionChanged,this,&Edge::updateAnimation);
}

// Деструктор класса
Edge::~Edge()
{
	delete timer;
	delete posAnim;
	delete it;
}

// Перерисовка линий при перетаскивании ребра
void Edge::adjust()
{
	QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
	qreal length = line.length();

	prepareGeometryChange();

    // Если расстояние между вершинами больше их радиуса
    if(length > qreal(node_diametr/2))
	{
		double angle = ::acos(line.dx() / line.length());
		if (line.dy() >= 0)
			angle = TwoPi - angle;
		QPointF c = 0.5*line.p1() + 0.5*line.p2() +QPointF(-sin(angle)*40,cos(Pi - angle)*40);


		QLineF l2(line.p1(),c);
		qreal length_3 = l2.length();

		QLineF l(c,line.p2());
		qreal length_2 = l.length();

		QPointF edgeOffset((l2.dx() * node_diametr/2) / length_3, (l2.dy() * node_diametr/2) / length_3);
		sourcePoint = line.p1() + edgeOffset;
		QPointF edgeOffset2((l.dx() * node_diametr/2) / length_2, (l.dy() * node_diametr/2) / length_2);
		destPoint = line.p2() - edgeOffset2;
	}
	else
		sourcePoint = destPoint =line.p1();
	source->update();
	dest->update();

    // Послать сигнал, что изменилась геометрия
    emit positionChanged();

}

// Установаить ограничивающую область фигуры для захвата курсором
QRectF Edge::boundingRect() const
{
    qreal extra = (2 + arrowSize) / 2.0;
	return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
									  destPoint.y() - sourcePoint.y()))
		.normalized()
		.adjusted(-extra, -extra, extra, extra);
}

// Установить форму объекта
QPainterPath Edge::shape() const
{
    if(sourcePoint == destPoint)
        return QPainterPath(sourcePoint);

    QLineF line(sourcePoint, destPoint);

    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;

    QPointF c = 0.5*line.p1() + 0.5*line.p2() +QPointF(-sin(angle)*40,cos(Pi - angle)*40);
    QPainterPath a;
    a.moveTo(sourcePoint);
    a.quadTo(c,destPoint);

    return a;
}

// Окраска ребра
void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	if(sourcePoint == destPoint)
		return;

	QLineF line(sourcePoint, destPoint);

	double angle = ::acos(line.dx() / line.length());
	if (line.dy() >= 0)
		angle = TwoPi - angle;

	painter->setBrush(Qt::NoBrush);


    // Отрисовки кривой
    painter->setPen(QPen(Qt::transparent, 22, style, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

	if(this->isSelected())
		painter->setPen(QPen(main_color, 4 , style, Qt::RoundCap, Qt::RoundJoin));
	else
		painter->setPen(QPen(main_color, 2, style, Qt::RoundCap, Qt::RoundJoin));

    painter->drawLine(line);
    //-----------------------------------------------------------------------------


    // Отрисовка конечной стрелки
    double ar_ang = ::acos(line.dx() / line.length());
    if(line.dy() >= 0)
		ar_ang = TwoPi - ar_ang;
	QPointF destArrowP1 = destPoint + QPointF(sin(ar_ang - Pi / 2.7) * arrowSize,
											  cos(ar_ang - Pi / 2.7) * arrowSize);
	QPointF destArrowP2 = destPoint + QPointF(sin(ar_ang - Pi + Pi / 2.7) * arrowSize,
											  cos(ar_ang - Pi + Pi / 2.7) * arrowSize);

    painter->setBrush(main_color);
	painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
    //-----------------------------------------------------------------------------


    // Отрисовка пропускной способности
    QPointF c = 0.5*line.p1() + 0.5*line.p2() +QPointF(-sin(angle)*33,cos(Pi - angle)*35) + QPointF(-25,-10);
    painter->drawText(QRectF(c,QSizeF(50,20)),Qt::AlignCenter,"["+ QString::number(value)+"]");
    //-----------------------------------------------------------------------------


    // Отрисовка текущего потока
    if(shouldShowCurrent)
	{
		painter->setPen(QPen(Qt::red,2));
		QPointF c = 0.5*line.p1() + 0.5*line.p2() +QPointF(-sin(angle)*10,cos(Pi - angle)*10) + QPointF(-25,-10);
		painter->drawText(QRectF(c,QSizeF(50,20)),Qt::AlignCenter,QString::number(currentValue));

	}
	update();
}

// Установить стиль ребра
void Edge::setStyle(Qt::PenStyle st)
{
	style = st;
	update();
}

// Установить текущий поток через данное ребро
bool Edge::setCurrent(int n)
{
    // Для отключения потока достаточно подать отрицательно число
    if(n < 0)
	{
		shouldShowCurrent = false;
		currentValue = 0;
		return false;
	}
	shouldShowCurrent = true;
	currentValue = n;

	return true;
}

// Текущий поток через данное ребро
int Edge::getCurrent() const
{
    return currentValue;
}

// Установка контексного меню при правом клике мышью
void Edge::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	QGraphicsItem::contextMenuEvent(event);
	setSelected(true);

    // Создание меню
    QMenu menu;
        QAction *  new_val = menu.addAction("Изменить пропускную способность");
	menu.addSeparator();
        QAction * remove_action = menu.addAction("Удалить ребро");

   // Выбор пункта меню
    QAction * selected_action = menu.exec(event->screenPos());


    // Выполнение действий
	if(selected_action == remove_action)
		scene->removeSelectedNodes();
	else if(selected_action == new_val)
    {
        bool correctInput;
        int t = QInputDialog::getInt(scene->getMGraph(), "Изменить пропускную способность",
                    "Пропускная способность:", value,1,1000000,1, &correctInput);
        if(correctInput)
			setValue(t);
	}

	setSelected(false);
}

// Установить пропускную способность
void Edge::setValue(int n)
{
	value = n;
}

// Пропускная способность
int Edge::getValue() const {return value;}

// Удаление ребра
void Edge::remove()
{
    dest->getEdges()->removeAll(this);
    source->getEdges()->removeAll(this);

    source->getChildren()->removeAll(dest);

	setAnimating(false);
	scene->removeItem(this);

    //заносим в контейнер с удаленными ребрами данное ребро
    //т.к. при удалении со сцены она больше не ответсвенная за удаление
    //объектов на ней.
    scene->deleted_it << this; // В деструкторе сцены этот контейнер очистится
	scene->performed_actions << MScene::smth_delted;
}

// Обработка двойного клика на ребро
void Edge::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mouseDoubleClickEvent(event);
    bool ok;
	int t = QInputDialog::getInt(scene->getMGraph(), "Пропуск. способн.",
            "Пропуск. способн.", value,1,1000000,1,&ok);
	if(ok)
		setValue(t);
}

// Установка цвета ребра
void Edge::setColor(QColor col)
{
    main_color = col;
	update();
}

// Получить вершину источник
Node * Edge::destNode() const
{
    return dest;
}

// Получить вершину ребенок
Node * Edge::sourceNode() const
{
    return source;
}

// Переопредление типа
int Edge::type() const
{
    return Type;
}


void Edge::deleteAnimation()
{
   // Удалить соединение с всплывающим текстом
   disconnect(timer,&QTimeLine::finished,this,&Edge::popupAnim);

   timer->stop();

   if(!it->pixmap().isNull()) // Удалить со сцены аним. объект
       scene->removeItem(it);

   if(!it->childItems().isEmpty())
       it->childItems().first()->setParentItem(Q_NULLPTR);

   it->setPixmap(QPixmap()); // Обнулить картинку объекта
}

void Edge::setAnimating(bool value)
{
    animating = value;

    if(!animating)
        deleteAnimation(); // Если false - удалить анимацию
    else
    {
        // Соединить заверешние таймера с всплывающей анимацией текста
        connect(timer,&QTimeLine::finished,this,&Edge::popupAnim);
        animate(); //анимировать
    }
}

bool Edge::isAnimating() const
{
    return animating;
}

void Edge::popupAnim()
{
    // Всплывающий текст
        QGraphicsTextItem * t = new QGraphicsTextItem("+" +QString::number(currentValue),this);

        t->setPos(destNode()->scenePos()+ QPointF(30,-30));
        t->setZValue(3.0);
        QTimeLine* timer = new QTimeLine(800);
        timer->setFrameRange(100,200);
        timer->setLoopCount(1);
        timer->setEasingCurve(QEasingCurve::OutQuad);

        // Эффект прозрачности для постепенного изчезновения объекта
        QGraphicsOpacityEffect * op = new QGraphicsOpacityEffect;
        t->setGraphicsEffect(op);

        // Соединяем таймер с лямбда-выражением, т.к. прозрачность меняется не по
        // easing curve таймера
        connect(timer,&QTimeLine::valueChanged,[op](qreal value)
                                                        { op->setOpacity(1.0-qPow(value,4));});
    // Анимационный объект
        QGraphicsItemAnimation * posAnim= new QGraphicsItemAnimation;
        posAnim->setTimeLine(timer);
        posAnim->setItem(t);
        posAnim->setPosAt(0.0,destNode()->scenePos()+ QPointF(30,-30));
        posAnim->setPosAt(1.0,destNode()->scenePos()+ QPointF(30,-80));

        timer->start(); //начало анимации всплывающего текста

    // Соединение завершения таймера и удаление всех созданных объектов
        connect(timer,&QTimeLine::finished,t,&QGraphicsTextItem::deleteLater);
        connect(timer,&QTimeLine::finished,posAnim,&QGraphicsItemAnimation::deleteLater);
        connect(timer,&QTimeLine::finished,op,&QGraphicsOpacityEffect::deleteLater);
        connect(timer,&QTimeLine::finished,timer,&QTimeLine::deleteLater);

        QPixmap p(":/cars/" +QString::number(qrand() % 8 + 1)
                                            +".png"
                            );
        it->setPixmap(p);
        it->setOffset(-p.width()/2.0,-p.height()/2.0);

        this->timer->start(); // Запустить заново основную анимацию передвижения

}

// Обновить координаты анимации при перетаскивании вершин
void Edge::updateAnimation()
{
    if(sourcePoint == destPoint)
        return;

    QLineF line(sourcePoint, destPoint); // Линия соединения вершин

    double angle = ::acos(line.dx() / line.length()); // Угол наклона основной линии соединения
    if (line.dy() >= 0)
        angle = TwoPi - angle;

    // Средняя точка основной линии со смещением
    QPointF c = 0.5*line.p1() + 0.5*line.p2() +QPointF(-sin(angle)*40,cos(Pi - angle)*40);

    QPainterPath a; // Построение кривой через три точки
    a.moveTo(sourcePoint);
    a.quadTo(c,destPoint);

    timer->setDuration(a.length()/0.1); // Расчет времени по длине ребра

    bool ce = true; // Для правильного расчета поворота
    for(int i = 0; i <= 10; i++)
    {
        qreal p = i/10.0;
        qreal p_pred =0;

        if(i>0)
            p_pred =(i-1)/10.0;

        if(a.angleAtPercent(p) > a.angleAtPercent(p_pred))
            ce = false;

        // Установка позиции в разный момент времени
        if(i==0)
            posAnim->setPosAt(p,sourceNode()->scenePos());
        else if(i==10)
            posAnim->setPosAt(p,destNode()->scenePos());
        else
            posAnim->setPosAt(p,a.pointAtPercent(p));

        // Установка поворота в разные моменты времени
        posAnim->setRotationAt(p, ce ? -a.angleAtPercent(p) : 360 - a.angleAtPercent(p));
    }

}

void Edge::animate()
{
    if(sourcePoint == destPoint)
        return;

    QPixmap p(":/cars/" +QString::number(qrand() % 8 + 1) +".png");

    it->setPixmap(p); // Установить изображение на объект
    it->setOffset(-p.size().width()/2.0,-p.size().height()/2.0); // Установка центра
    it->setZValue(0.5);
    it->setPos(sourceNode()->scenePos());
    it->setTransformationMode(Qt::SmoothTransformation);

    posAnim->setItem(it);

    updateAnimation();
    scene->addItem(it);
    timer->start();

}
