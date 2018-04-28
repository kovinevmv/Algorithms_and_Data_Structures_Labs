#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>
class Options;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    friend class Options;

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_action_7_triggered();

    void on_action_8_triggered();

    void on_action_2_triggered();

    void on_action_triggered();

    void on_action_1_triggered();

    void on_action_13_triggered();

    void on_action_14_triggered();

    void on_action_15_triggered();

    void on_pushButton_clicked();

    void on_action_19_triggered();

    void on_action_20_triggered();

    void on_action_grph_3_triggered();

    void openFile(QString temp);

    QString saveFile();

    void on_action_3_triggered();

    void on_action_21_triggered();

    void on_action_zgrph_2_triggered();

    void on_action_6_triggered();

private:
    Ui::MainWindow *ui;

    bool processing;
    int n,e;
    int **capacity; // Матрица пропускных способнотей
    int ** flow;  // Матрица потока
    int * color;      // Цвета для вершин
    int * pred;       // Массив пути
    int head, tail;  // Начало, Конец
    int * q;      // Очере

    void enque(int x);
    int deque();
    int bfs(int start,int end);
    int max_flow(int source,int stock);
};

#endif // MAINWINDOW_H
