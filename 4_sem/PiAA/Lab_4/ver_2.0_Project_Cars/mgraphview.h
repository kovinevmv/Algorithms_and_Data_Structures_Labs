#ifndef MGRAPHVIEW_H
#define MGRAPHVIEW_H
#include <QtWidgets>

class MScene;


//виджет для отображения сцены
class MGraphView : public QGraphicsView
{
public:

	MGraphView(QWidget *parent = 0); //стандартный конструктор
	MScene* getMscene(){return scene;} //получить указатель на сцену

	~MGraphView();
protected:
	void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE; //для обработки нажатия клавиш

	void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE; //для обработки вращения колесика

//переопределенные методы для реализации открытия перетаскиванием
	void dragEnterEvent(QDragEnterEvent *event)Q_DECL_OVERRIDE;
	void dropEvent(QDropEvent *event)Q_DECL_OVERRIDE;
	void dragLeaveEvent(QDragLeaveEvent *event)Q_DECL_OVERRIDE;
	void dragMoveEvent(QDragMoveEvent *event)Q_DECL_OVERRIDE;
	//void contextMenuEvent(QContextMenuEvent * event);

private:
	MScene* scene; //указатель на сцену

	void scaleView(qreal scaleFactor); //увеличение масштаба

};

#endif // MGRAPHVIEW_H
