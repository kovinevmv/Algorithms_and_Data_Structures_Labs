#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    vector<vector<MyNode>> textEditToVector();
    void on_pushButton_3_clicked();
    void updateMatrix(const vector<vector<MyNode>>& );
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_action_triggered();
    void on_action_2_triggered();
    void on_action_4_triggered();
    void on_action_3_triggered();
    void on_pushButton_7_clicked();
    void on_radioButton_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_action_5_triggered();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
