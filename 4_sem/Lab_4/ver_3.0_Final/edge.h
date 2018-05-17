#ifndef EDGE_H
#define EDGE_H
#include <QGraphicsItem>
#include <QtWidgets>

class Node;
class MScene;

// Класс для описания ребер графа
class Edge : public QObject, public QGraphicsItem
{
        Q_OBJECT                        // Для реализация слотов и сигналов
        Q_INTERFACES(QGraphicsItem)

    public:
        // Конструктор принимает вершину родителя, вершину ребенка, вес ребра (пропускную способность)
        Edge(Node *sourceNode, Node *destNode, MScene *sc, int val = 1,QObject * obj = Q_NULLPTR);

        // Деструктор класса
        ~Edge();

        void setColor(QColor col);                      // Установка цвета ребра
        void setValue(int n);                           // Установить пропускную способность
        void setStyle(Qt::PenStyle st = Qt::SolidLine); // Установить стиль ребра
        bool setCurrent(int n);                         // Установить текущий поток через данное ребро

        int getCurrent() const;                         // Текущий поток через данное ребро
        int getValue() const;                           // Пропускная способность

         void remove();               // Удалить ребро

        Node *sourceNode() const;     // Получить вершину источник
        Node *destNode()   const;	  // Получить вершину ребенок

        void adjust();                // Перерисовка линий при перетаскивании ребра

        void setAnimating(bool);      // Отображать ли анимацию
        bool isAnimating() const;     // Производится ли на ребре анимация


        enum { Type = UserType + 2 };
        int type() const Q_DECL_OVERRIDE; // Переопредление типа

    protected:
         // Окраска ребра
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
        QRectF boundingRect() const Q_DECL_OVERRIDE; // Установаить ограничивающую область фигуры для захвата курсором

        QPainterPath shape() const Q_DECL_OVERRIDE; // Установить форму объекта

         // Установка контексного меню при правом клике мышью
        void contextMenuEvent(QGraphicsSceneContextMenuEvent *event)Q_DECL_OVERRIDE;

        // Обработка двойного клика на ребро
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
private:
        //Соединяемые вершины
        Node	*source, *dest;

        //Координаты концов линии
        QPointF sourcePoint, destPoint;

        MScene* scene;       // Сцена, в которой находится ребро

        qreal			arrowSize;    // Размер стрелки
        int				node_diametr; // Диаметр вершины
        QColor			main_color;   // Цвет
        Qt::PenStyle	style;        // Стиль

        int		value;               // Пропускная способность
        int		currentValue;        // Текущий поток

        bool shouldShowCurrent; // Отображать ли текущий поток

        QGraphicsPixmapItem		*it;       // Объект на ребре
        QGraphicsItemAnimation	*posAnim;  // Анимация объекта it
        QTimeLine				*timer;    // Таймер для анимации
        bool					animating; // Анимируется ли ребро

        void animate();  // Анимировать ребро
        void deleteAnimation(); // Удалить анимацию

    private slots:

        void updateAnimation(); // Обновить координаты анимации при перетаскивании вершин
        void popupAnim();       // Анимация всплывающего текста над вершиной-приемником

    signals:

        void positionChanged(); // Сигнал о том, что изменилась геометрия

};

#endif // EDGE_H
