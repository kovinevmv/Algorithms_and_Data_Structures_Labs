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

// Окно графа
class GraphWindow : public QWidget
{
	Q_OBJECT
public:
    explicit GraphWindow(QWidget *parent = 0); // Конструктор класса
    ~GraphWindow();                            // Деструктор класса

    MScene * getMscene();                      // Сцена
    bool getProcessing() const;                // Проверка работы программы

    bool createNewGraph(QString temp);         // Создать граф из списка инц.
    bool createNewGraphWithFormat(QString);    // Создать форматированный граф из файла
    bool createNewGraphWithNewFormat(QString); // Создать формат. граф из сжатого файла

    void writePathSettings(QString str);       // Запись и чтение настроек о
    QString readPathSettings();			       // пути сохранения и открытия

    QString textToGraphParse(QString);         // Парсинг строки в граф
    QString graphToTextParse();                // Парсинг графа с строку

public slots:

    void saveTextGraph();             // Сохранить граф как текст
    void saveGraphWithFormat();       // Сохранить граф как формат. файл
    void saveGraphWithNewFormat();    // Сохранить граф как формат. сжатый файл

    void openTextGraph();             // Открыть граф как текст
    void openGraphWithFormat();       // Открыть граф как формат. файл
    void openGraphWithNewFormat();    // Открыть граф как формат. сжатый файл

    bool example1Graph();             // Создание примеров графа
    bool example2Graph();             // Создание примеров графа

    void stopAnimationButton();   // Кнопка остановки анимации
    void stopAnim();              // Остановить всю анимацию на сцене

    void startSearchMaxFlow();    // Запуск поиска максимального потока


private:
    //Функция форматирования входных строк под один формат
    QString add_spaces_and_simplifie(QString str_for_work);

    void setFormat();   //Установить форматирование графа (цвет и стиль ребер)
    void clearFormat(); //Очистить форматирование графа (цвет и стиль ребер)

    void writeSettings();	// Установка настроек пути файлов
    void readSettings();

    QList<QString> nodes;
    QList<QStringList> child_of_nodes;	   // Контейнеры с информацией
                                           // необходимой для создания
    QList<QPointF> points_of_nodes;		   // форматированного графа из
    QList<QColor> color_of_nodes;          // файла типа .grph
    QList<QStringList> values_of_edges;

    bool processing;              // Выполняется ли алгоритм
    int  msec_delay;              // Задержка в милисекундах
    bool paused {false};          // Пауза

    int size;        // Количество вершин
    int **capacity;  // Матрица пропускных способнотей
    int ** flow;     // Матрица потока
    int * pred;      // Массив пути

    bool bfs(int, int);       // Функция поиска в ширину
    int max_flow(int, int);   // Функция поиск максимального потока

    Ui::GraphWindow *ui;
    QString log;

private slots:
    void showTextGraph(QString); // Отобразить текстовое представление графа (список инцид.)

    void cleanGraph();  // Очистить граф

    void showCreateHints(); // Справка по созданию
    void showTokHints();	// и по алгоритму макс. потока

    void convertToUnoriented(); // Перевести в неориентир.
    void selectAll();			// Выделить всё

    void maxFlowInit();  // Инициализация алгоритма поиска макс. потока

protected:
    void closeEvent(QCloseEvent * event); // Событие закрытия окна

};

#endif // GRAPHWINDOW_H
