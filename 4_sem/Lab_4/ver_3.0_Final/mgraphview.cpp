#include <math.h>

#include "mgraphview.h"
#include "mscene.h"
#include "graphwindow.h"

MGraphView::MGraphView(QWidget *parent)
	: QGraphicsView(parent)
{
    // Создать и установить сцену
    scene = new MScene(this);
	setScene(scene);

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
    // Обработка нажатий клавиш
	switch (event->key())
	{
    // Удаление
    case Qt::Key_Delete: case Qt::Key_X:
		if(!scene->selectedItems().isEmpty())
			scene->removeSelectedNodes();
		break;

    // Изменение цвета
    case Qt::Key_C:
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
    // Ограничение размеров
    if (factor < 0.1 || factor > 5)
		return;

	scale(scaleFactor, scaleFactor);
}

void MGraphView::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasText()) //если перетаскиваемый тип - текст
        event->accept();			 //принять объект
	else
		event->ignore();
}

void MGraphView::dropEvent(QDropEvent *event)
{
        QString fileNamePath = event->mimeData()->text().remove("file:///");

        GraphWindow *main_graph = dynamic_cast<GraphWindow *>(parent());

        // Проверка кооректности открываемого файла
        if (fileNamePath.indexOf(".grph") != -1  || fileNamePath.indexOf(".zgrph") != -1  ) //если формат файла .grph - Открыть
		{
            if(main_graph->getProcessing())
			{
                QMessageBox::information(main_graph,"Ошибка!", "В данный момент обрабатывается другой граф!");
				return;
			}

            // Установка QProgressDialog
            QProgressDialog *progressBarLoadingFile = new QProgressDialog(main_graph);
            progressBarLoadingFile->setAttribute(Qt::WA_DeleteOnClose );
            progressBarLoadingFile->setWindowModality(Qt::ApplicationModal);
            progressBarLoadingFile->setWindowFlags(Qt::Window|Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
            progressBarLoadingFile->setWindowTitle("Открытие");
            progressBarLoadingFile->setLabelText("Идет открытие файла...");
            progressBarLoadingFile->setRange(0,10);
            progressBarLoadingFile->setCancelButton(Q_NULLPTR);
            progressBarLoadingFile->show();

            main_graph->stopAnim();
			for(int i = 0; i < 10;i++)
			{
                progressBarLoadingFile->setValue(i);
				MScene::setDelay(100);
			}
            progressBarLoadingFile->close();


            // Открытие файла
            if (fileNamePath.indexOf(".grph") != -1)
                main_graph->createNewGraphWithFormat(fileNamePath);
            else
                main_graph->createNewGraphWithNewFormat(fileNamePath);

		}
        else
            // Вывод ошибки
            QMessageBox::information(main_graph, "Ошибка!", "Формат файла должен быть .grph или .zgrph!");

}

void MGraphView::dragMoveEvent(QDragMoveEvent *event){Q_UNUSED(event)}

void MGraphView::dragLeaveEvent(QDragLeaveEvent *event){Q_UNUSED(event);}
