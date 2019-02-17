#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>

class Edge;
class MGraphView;
class MScene;

// Класс вершин графа
class Node : public QGraphicsItem
{
    public:
        Node(MScene*,  QString val = "");  // Контсруктор класса Node

        QColor  getColor() const;     // Функиця, возвращающая цвет вершины
        QString getValue() const;     // Функиця, возвращающая имя вершины

        void setColor(QColor);   // Установка цвета вершины
        void setValue(QString);  // Установка имени вершины

        void remove();           // Удаление вершины

        void addChild(Node*);    // Добавление наследника (смежные вершины)
        void addEdge(Edge*);     // Добавление ребра в список ребер

        // Соединены ли вершины
        static bool isNodesConnected(Node*, Node* );

        enum { Type = UserType + 1 };
        int type() const Q_DECL_OVERRIDE;    // Переопредление типа

        QList<Edge *> * getEdges();      // Функиця, возвращающая список вершин
        QList<Node *> * getChildren();   // Функция, возвращащающая список наследников

        // Оператор<< для вершины
        friend QString& operator<<(QString&, const Node&);

   protected:

        // Окраска вершины
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget*) Q_DECL_OVERRIDE;
        QRectF boundingRect() const Q_DECL_OVERRIDE;   // Установаить ограничивающую область фигуры для захвата курсором
        QPainterPath shape()  const Q_DECL_OVERRIDE;   // Установить форму объекта - окружность

        // Именение положения вершины
        QVariant itemChange(GraphicsItemChange , const QVariant&) Q_DECL_OVERRIDE;

         // Установка контексного меню при правом клике мышью
        void contextMenuEvent(QGraphicsSceneContextMenuEvent*)Q_DECL_OVERRIDE;
        // Обработка двойного клика на вершину
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*)Q_DECL_OVERRIDE;
    private:

        QList<Edge *> edgeList;        // Список ребер
        QList<Node *> childrenList;    // Смежные вершины

        MScene * scene;                // Сцена, в которой находится вершина

        QColor main_color {QColor(255,235,0)};    // Цвет
        QString value;	                          // Значение (имя)
        int diametr{60};                          // Диаметр
};


#endif // NODE_H
