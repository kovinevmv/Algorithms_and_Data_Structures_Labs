#ifndef MSCENE_H
#define MSCENE_H
#include <QtWidgets>

class GraphWindow;
class MGraphView;
class Node;
class Edge;
class MainWindow;
class Options;
//Класс со сценой
class MScene : public QGraphicsScene
{
    friend class GraphWindow;
    friend class MGraphView;
    friend class Node;
    friend class Edge;
    friend class MainWindow;
    friend class Options;
public:
    MScene(QObject* par = 0);
    ~MScene();

    //добавить вершину в заданной точке с заданным значением
    void addNode(QPointF position, QString value = "");

    //добавить ребро к заданным вершинам с заданным весом
    void addEdge(Node* source, Node* dest, int val=1);

    //установить диаметр для вершин
    void setDiametr(int d);

    //сделать граф ориентированным/ неориентированным
    void setOrientated(bool b);

    //установить размер стрелок
    void setArrowSize(int d);

    //включить/выключить отображение весов ребер
    void setShowEdge(bool a);

    int diametr() const;
    int arrSize() const {return arrow_size;}
    bool shouldShowValues() const;
    bool isOriented() const;

    QList<Node*> * nodesList();                         //список всех вершин
    Node* findNode(QString val);                        //найти вершину по значению
    Edge* findEdge(Node* source,Node* dest);            //найти ребро по вершинам
    MGraphView * getMGraph();                           //виджет

private:
    void setColorOfSelectedNodes();                     //Установка цвета выделенных вершин

    //создание графа
    void createGraphWithText(QList<QString> & nodes, QList<QStringList> & child_of_nodes);

    QList<Node*> all_nodes;                             //все вершины на сцене
    QList<QGraphicsItem*> deleted;                      //все удаленные со сцены объекты (удаляются позже)
    MGraphView * parent;                                // виджет

    Node* first;
    QGraphicsItem * second;

    bool show_edge_values;                              //показывать ли веса ребер
    bool graph_oriented;                                //ориентированный ли граф

    int node_diametr;                                   //диаметр вершин
    int arrow_size;                                     //размер стрелки

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

private slots:
    //соединить две вершины с помощью мыши
    void connectTwoNodes();

public slots:
    //удаление выделенных объектов
    void removeSelectedNodes();
};

#endif // MSCENE_H
