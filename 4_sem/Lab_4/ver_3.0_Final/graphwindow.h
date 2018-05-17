#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H
#include <QWidget>
#include <queue>
#include <QtWidgets>
class MScene;
class Node;

namespace Ui
{
    class GraphWindow;
}

//окно  графа
class GraphWindow : public QWidget
{
	Q_OBJECT
public:
	explicit GraphWindow(QWidget *parent = 0);
	~GraphWindow();
	MScene * getMscene(); //сцена

	bool createNewGraph(QString temp); //создать граф из списка инц.
    bool createNewGraphWithFormat(QString); //создать форматированный граф из файла
    bool createNewGraphWithNewFormat(QString);
	void writePathSettings(QString str); //запись и чтение настроек о
	QString readPathSettings();			 //пути сохранения и открытия

	bool getProcessing() const;
    QString textToGraphParse(QString ); // parse string
    QString graphToTextParse();

public slots:
    void saveTextGraph();//текст
    void openTextGraph();

	void saveGraphWithFormat(); //форматир. граф
    void openGraphWithFormat();

    void saveGraphWithNewFormat();
    void openGraphWithNewFormat();

    bool createRandomGraph();
    bool example1Graph();
    bool example2Graph();
	void stopAnim();  //остановить всю анимацию на сцене

    void startSearchMaxFlow();
    void stopAnimationButton();

private:
	QList<QString> nodes;				//контейнеры с информацией
	QList<QStringList> child_of_nodes;	//необходимой для создания
										//форматированного графа из
	QList<QPointF> points_of_nodes;		//файла типа .grph
	QList<QColor> color_of_nodes;
	QList<QStringList> values_of_edges; //

	bool processing; //выполняется ли алгоритм
	int msec_delay;  //задержка в милисекундах
	bool paused{false};

	QString add_spaces_and_simplifie(QString str_for_work); //для форматирования входных строк под один формат

	void setFormat();
	void clearFormat(); //очистить форматирование графа (цвет и стиль ребер)

	void writeSettings();	//установка настроек
	void readSettings();

	int size;  //кол-во вершин
	int **capacity; // Матрица пропускных способнотей
	int ** flow;  // Матрица потока
	int * pred;       // Массив пути

	bool bfs(int start,int end); //поиск в ширину
	int max_flow(int source,int stock); //поиск макс. потока

    Ui::GraphWindow *ui;
    QString log;

private slots:
    void showTextGraph(QString); //отобразить текстовое представление графа (список инцид.)

    void cleanGraph();  //очистить граф

	void showCreateHints(); //справка по созданию
	void showTokHints();	//и по алгоритму макс. потока

	void convertToUnoriented(); //перевести в неориентир.
	void selectAll();			//выделить всё.

	void maxFlowInit();  //инициализация алгоритма поиска макс. потока

protected:
	void closeEvent(QCloseEvent * event); //событие закрытия окна

};

#endif // GRAPHWINDOW_H
