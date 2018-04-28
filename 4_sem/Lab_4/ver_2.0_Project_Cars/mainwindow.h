#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "graphwindow.h"


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
	void on_create_graph_triggered();

	void on_open_graph_triggered();

	void on_auth_info_triggered();

	void on_open_format_graph_triggered();

	void on_program_info_triggered();

	void on_action_3_triggered();

	void on_action_4_triggered();

	void on_action_2_triggered();

	void on_create_simple_graph_clicked();

private:

	GraphWindow* window_with_graph{Q_NULLPTR};

	Ui::MainWindow *ui;


};

#endif // MAINWINDOW_H
