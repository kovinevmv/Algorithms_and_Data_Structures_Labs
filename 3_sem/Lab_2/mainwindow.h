#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "list.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    data_container *data;

private slots:

    void on_inputText_textChanged();
    void on_actionButton_clicked();
    void print(bool);
    void generateComboBox();
    void on_action_3_triggered();
    void on_action_4_triggered();
    void on_action_triggered();
    void on_action_2_triggered();
};



#endif // MAINWINDOW_H
