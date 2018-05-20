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
    friend class GraphWindow;  // Установка дружественных классов
    friend class Edge;
    friend class Node;
public:
        MScene(QObject* par = 0);  // Конструктор класса
        ~MScene();                 // Деструктор класса

        void setColorOfSelectedNodes(); // Установка цвета
        void removeSelectedNodes();     // Удаление выбранных вершин

        // Cоздать граф
        void createGraphWithText(QList<QString> & nodes, QList<QStringList> & child_of_nodes);

        QList<Node*>*nodesList(); // Указатель на контейнер с вершинами

        Node* addNode(QPointF position, QString value = "");   // Добавить вершину
        Edge* addEdge(Node* source, Node* dest, int val = 1);  // Добавить ребро

        Edge* findEdge(Node* source,Node* dest);  // Найти на сцене ребро
        Node* findNode(QString val);              // Найти на сцене вершину

        MGraphView * getMGraph() const; // Виджет, в котором находится сцена
        static void setDelay(int msec); // Фукнция задержки на msec миллисекунд
    private:

        QList<Node*> all_nodes; // Контейнер с текущими вершинами на сцене
        Node *first;            // Указатели, необходимые
        QGraphicsItem* second;  // для метода connectTwoNodes

        MGraphView *parent;  // Виджет отображения данной сцены

        // Копирование
        QList<Node*> copy_n; //скопированные элементы
        QList<Edge*> copy_e;

        QList<QGraphicsItem*> deleted_it; // Контейнер с удаленными вершинами

        enum action {smth_delted,smth_added};
        QVector<action> performed_actions;
    protected:
        // Обработка двойного клика на ребро
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event) Q_DECL_OVERRIDE;
        void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
         // Установка контексного меню при правом клике мышью
        void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) Q_DECL_OVERRIDE;

    private slots:
        void connectTwoNodes();  // Соединить две вершины с помощью мыши

    public slots:
        void copySelectedNodes(Node* startNode = Q_NULLPTR); // Скопировать выделенное
        void paste(QPointF p = QPoint(0,0));                 // Вставить элементы

};

#endif // MSCENE_H
