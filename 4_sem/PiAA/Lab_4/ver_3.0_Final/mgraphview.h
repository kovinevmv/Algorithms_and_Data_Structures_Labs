#ifndef MGRAPHVIEW_H
#define MGRAPHVIEW_H
#include <QtWidgets>

class MScene;

// Виджет для отображения сцены
class MGraphView : public QGraphicsView
{
public:

    MGraphView(QWidget *parent = 0);     // Стандартный конструктор
    MScene* getMscene(){return scene;}   // Получить указатель на сцену

	~MGraphView();
protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE; // Для обработки нажатия клавиш
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;  // Для обработки вращения колесика

    // Переопределенные методы для реализации открытия перетаскиванием
	void dragEnterEvent(QDragEnterEvent *event)Q_DECL_OVERRIDE;
	void dropEvent(QDropEvent *event)Q_DECL_OVERRIDE;
	void dragLeaveEvent(QDragLeaveEvent *event)Q_DECL_OVERRIDE;
	void dragMoveEvent(QDragMoveEvent *event)Q_DECL_OVERRIDE;

private:
    MScene* scene; // Указатель на сцену
    void scaleView(qreal scaleFactor); // Увеличение масштаба
};

#endif // MGRAPHVIEW_H
