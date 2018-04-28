#include <math.h>

#include "mgraphview.h"
#include "mscene.h"
#include "graphwindow.h"

MGraphView::MGraphView(QWidget *parent)
	: QGraphicsView(parent)
{
	scene = new MScene(this); //создать и установить сцену
	setScene(scene);
	//scene->setSceneRect(-width()/2.0,-height()/2.0,width(),height());

	setViewportUpdateMode(FullViewportUpdate);
	setOptimizationFlags(DontSavePainterState | DontClipPainter | DontAdjustForAntialiasing);
	setRenderHint(QPainter::Antialiasing, true);

	scale(qreal(1), qreal(1));
	setWindowIcon(QIcon(":/icons/graph.png"));
	setAcceptDrops(true);
	setDragMode(RubberBandDrag);
	setTransformationAnchor(AnchorUnderMouse);
}

MGraphView::~MGraphView()
{
	delete scene;
}

void MGraphView::keyPressEvent(QKeyEvent *event)
{
//обработка нажатий клавиш
	switch (event->key())
	{
	case Qt::Key_Delete: case Qt::Key_X: //удаление
		if(!scene->selectedItems().isEmpty())
			scene->removeSelectedNodes();
		break;

	case Qt::Key_C: //изменение цвета
		if(!scene->selectedItems().isEmpty())
			scene->setColorOfSelectedNodes();
		break;

	default:
		break;
	}
		QGraphicsView::keyPressEvent(event);
}

void MGraphView::wheelEvent(QWheelEvent *event)
{
	scaleView(pow((double)2, event->delta() / 240.0));
}


void MGraphView::scaleView(qreal scaleFactor)
{
	qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor < 0.1 || factor > 5) //ограничение размеров
		return;

	scale(scaleFactor, scaleFactor);
}


void MGraphView::dragEnterEvent(QDragEnterEvent *event)
{
	if(event->mimeData()->hasText()) //если перетаскиваемый тип - текст
		event->accept();			//принять объект
	else
		event->ignore();
}

void MGraphView::dropEvent(QDropEvent *event)
{
		QString str = event->mimeData()->text().remove("file:///");

		GraphWindow * p = dynamic_cast<GraphWindow *>(parent());

		if (str.indexOf(".grph") != -1 ) //если формат файла .grph - Открыть
		{
			if(p->getProcessing())
			{
				QMessageBox::information(p,"Ошибка!", "В данный момент обрабатывается другой граф!");
				return;
			}
			QProgressDialog *sl = new QProgressDialog(p);
			sl->setAttribute(Qt::WA_DeleteOnClose );
			sl->setWindowModality(Qt::ApplicationModal);
			sl->setWindowFlags(Qt::Window|Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
			sl->setWindowTitle("Открытие");
			sl->setLabelText("Идет открытие файла...");
			sl->setRange(0,10);
			sl->setCancelButton(Q_NULLPTR);
			sl->show();

			p->stopAnim();
			for(int i = 0; i < 10;i++)
			{
				sl->setValue(i);
				MScene::setDelay(100);
			}
			sl->close();
			p->createNewGraphWithFormat(str);

		}
		else //выдать ошибку иначе
			QMessageBox::information(p,"Ошибка!", "Формат файла должен быть .grph!");

}

void MGraphView::dragMoveEvent(QDragMoveEvent *event){Q_UNUSED(event)}

void MGraphView::dragLeaveEvent(QDragLeaveEvent *event){Q_UNUSED(event);}




