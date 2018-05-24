#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graphwidget.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_textEdit_textChanged();

    void on_pushButton_2_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void bsearchBinTree(binTree, int, QString);

    void on_pushButton_3_clicked();

    void on_action_2_triggered();

    void on_action_triggered();

    void on_action_3_triggered();

    void on_action_4_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
