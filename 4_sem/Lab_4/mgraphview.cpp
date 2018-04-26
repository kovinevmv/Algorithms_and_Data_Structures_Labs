#include <math.h>

#include "mgraphview.h"
#include "mscene.h"

MGraphView::MGraphView(QWidget *parent)
	: QGraphicsView(parent)
{
	scene = new MScene(this);
	setScene(scene);

	setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	setOptimizationFlags(DontSavePainterState | DontClipPainter | DontAdjustForAntialiasing);
	setRenderHint(QPainter::Antialiasing, true);

	scale(qreal(1), qreal(1));
	setAcceptDrops(true);
	setDragMode(QGraphicsView::RubberBandDrag);
	setTransformationAnchor(AnchorUnderMouse);

}
MGraphView::~MGraphView()
{
	delete scene;
}

//обработка событий клавиш
void MGraphView::keyPressEvent(QKeyEvent *event)
{

	switch (event->key()) {
    case Qt::Key_Delete:
    case Qt::Key_X:
		if(!scene->selectedItems().isEmpty())
			scene->removeSelectedNodes();
		break;
	case Qt::Key_C:
		if(!scene->selectedItems().isEmpty())
			scene->setColorOfSelectedNodes();
		break;
	default:
		break;
	}
		QGraphicsView::keyPressEvent(event);
}

//обработка колеса мыши
void MGraphView::wheelEvent(QWheelEvent *event)
{
	scaleView(pow((double)2, event->delta() / 240.0));
}

//масштабирование
void MGraphView::scaleView(qreal scaleFactor)
{
	qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor < 0.07 || factor > 100)
		return;

	scale(scaleFactor, scaleFactor);
}
