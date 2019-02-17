#ifndef MGRAPHVIEW_H
#define MGRAPHVIEW_H
#include <QtWidgets>

class MScene;
//Класс с виджетом
class MGraphView : public QGraphicsView
{
public:

	MGraphView(QWidget *parent = 0);
	MScene* getMscene(){return scene;}

	~MGraphView();
protected:
	void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
	void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

private:
	void scaleView(qreal scaleFactor);
//увеличение масштаба

	MScene* scene;
};

#endif // MGRAPHVIEW_H
