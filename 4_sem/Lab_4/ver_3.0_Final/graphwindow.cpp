#include "graphwindow.h"
#include "ui_graphwindow.h"

#include "qcompressor.h"
#include "mscene.h"
#include "node.h"
#include "edge.h"
#include "informationdialog.h"
#include <mgraphview.h>
#include <ctime>

//окно с графом
GraphWindow::GraphWindow(QWidget *parent) :
	QWidget(parent),
    processing(false),msec_delay(600), ui(new Ui::GraphWindow)
{

    ui->setupUi(this);

    ui->main_graph->setParent(this);
    setAttribute(Qt::WA_DeleteOnClose, true); // Удалять окно при его закрытии
    readSettings(); // Прочитать настройки с геометрией окна

    QString styleSheetMenu = "QMenu::separator {height: 3px; background: darkMagenta; "
                             "margin-left: 12px; margin-right: 6px;}";

	QMenu * file =  new QMenu("Файл");
    file->setStyleSheet(styleSheetMenu);

    QMenu *simpleFile =  new QMenu("Простой файл");
    simpleFile->addAction("Открыть список (.txt)", this, SLOT(openTextGraph()), QKeySequence("Ctrl+Shift+S"));
    simpleFile->addAction("Сохранить как текст (.txt)", this, SLOT(saveTextGraph()),QKeySequence("Ctrl+S"));
    simpleFile->setStyleSheet(styleSheetMenu);
    file->addMenu(simpleFile);

    QMenu *formatredFile =  new QMenu("Форматированный файл");
    formatredFile->addAction("Открыть (.grph)", this, SLOT(openGraphWithFormat()), QKeySequence("Ctrl+Shift+F"));
    formatredFile->addAction("Сохранить (.grph)", this, SLOT(saveGraphWithFormat()),QKeySequence("Ctrl+F"));
    formatredFile->setStyleSheet(styleSheetMenu);
    file->addMenu(formatredFile);

    QMenu *newFormattedFile =  new QMenu("Новый форматированный файл");
    newFormattedFile->addAction("Открыть (.zgrph)", this, SLOT(openGraphWithNewFormat()), QKeySequence("Ctrl+Shift+Z"));
    newFormattedFile->addAction("Сохранить (.zgrph)", this, SLOT(saveGraphWithNewFormat()),QKeySequence("Ctrl+Z"));
    newFormattedFile->setStyleSheet(styleSheetMenu);
    file->addMenu(newFormattedFile);
    file->addSeparator();
    file->addAction("Выход",this,SLOT(close()));

    QMenu* workWithGraph = new QMenu("Параметры");
    workWithGraph->setStyleSheet(styleSheetMenu);
    QAction* a  = workWithGraph->addAction("Копировать");
    a->setShortcut(QKeySequence("Ctrl+C"));
    connect(a, &QAction::triggered, [this](){getMscene()->copySelectedNodes();});
    QAction* b  = workWithGraph->addAction("Вставить");
    b->setShortcut(QKeySequence("Ctrl+V"));
    connect(b, &QAction::triggered, [this](){getMscene()->paste();});

    workWithGraph->addAction("Очистить все",this, SLOT(cleanGraph()));

    QMenu* flowFind = new QMenu("Максимальный поток");
    flowFind->setStyleSheet(styleSheetMenu);
    flowFind->addAction("Найти максимальный поток", this, SLOT(startSearchMaxFlow()), QKeySequence("F5"));
    flowFind->addAction("Очистка анимации", this, SLOT(stopAnimationButton()));

    QMenu* examples = new QMenu("Примеры");
    examples->setStyleSheet(styleSheetMenu);
    examples->addAction("Генерация", this, SLOT(createRandomGraph()), QKeySequence("Ctrl+G"));
    examples->addAction("Поток №1",  this, SLOT(example1Graph()));
    examples->addAction("Поток №2",  this, SLOT(example2Graph()));

    QMenu* info = new QMenu("Справка");
    info->setStyleSheet(styleSheetMenu);
    info->addAction("Справка",this,SLOT(showCreateHints()), QKeySequence("F1"));
    info->addAction("Об авторе",this,SLOT(showTokHints()), QKeySequence("F2"));

	QMenuBar * menuBar = new QMenuBar; //строка с меню
    menuBar->setStyleSheet("QMenuBar::item:selected {background: #606060;");
    menuBar->addMenu(file);
    menuBar->addMenu(workWithGraph);
    menuBar->addMenu(flowFind);
    menuBar->addMenu(examples);
    menuBar->addMenu(info);

    ui->gridLayout->setMenuBar(menuBar);
	setWindowTitle("Редактор графов");
	setWindowIcon(QIcon(":/icons/graph.png"));
}

GraphWindow::~GraphWindow()
{
	delete ui;
}

bool GraphWindow::getProcessing() const
{
	return processing;
}

void GraphWindow::selectAll()
{
	foreach (QGraphicsItem * node, getMscene()->items()) {
        node->setSelected(true); // Выделить каждый элемент
	}
}

void GraphWindow::showTextGraph(QString x)
{
    InformationDialog * out = new InformationDialog(x,this);
    out->show(); // Вывод строки со списком инцидентности
}

void GraphWindow::saveTextGraph() // Сохранение в текстовом виде
{
	QString a;
    foreach (Node* node, *(ui->main_graph->getMscene()->nodesList())){
		a << *node;
		a.append("\n");}

	if(a.size() == 0)
		return;

	QString FileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"),readPathSettings(),
													tr("text(*.txt)"));
	if (FileName.isEmpty())
		return;

	QFile File(FileName);
	if(File.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QByteArray Data;
		Data.append(a);
		File.write(Data);
		File.close();
	}
}

void GraphWindow::openTextGraph() // Открыть обычный граф из списка инцидентности
{
    stopAnim();
    cleanGraph();
    QString FileName = QFileDialog::getOpenFileName(this, "Открыть файл...", readPathSettings(), "Text files (*.txt)");
    if (FileName.isEmpty())
        return;

    writePathSettings(FileName);
    QFile File(FileName);

    if(File.open(QIODevice::ReadOnly | QIODevice::Text))
    {

        createNewGraph(File.readAll());
        show();
        File.close();
    }
}

void GraphWindow::saveGraphWithFormat() // Сохранение файла с форматированием (цвет, позиция, значения вершин и пропускных способностей)
{
    QString a = graphToTextParse();

	if(a.size() == 0)
		return;

	QString FileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"),readPathSettings(),
													tr("Graph File(*.grph)"));
	if (FileName.isEmpty())
		return;

	QFile File(FileName);
	if(File.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QByteArray Data;
		Data.append(a);
		File.write(Data);
		File.close();
	}
}

void GraphWindow::openGraphWithFormat() // Открыть форматированный граф
{
    stopAnim();
    cleanGraph();
    QString FileName = QFileDialog::getOpenFileName(this, "Открыть файл...", readPathSettings(), "graph file (*.grph)");
    if (FileName.isEmpty())
        return;

    writePathSettings(FileName);
    createNewGraphWithFormat(FileName);
    show();
}

void GraphWindow::openGraphWithNewFormat() //открыть форматированный граф
{
    stopAnim();
    cleanGraph();
    QString FileName = QFileDialog::getOpenFileName(this, "Открыть файл...", readPathSettings(), "ZGraph file (*.zgrph)");
    if (FileName.isEmpty())
        return;

    writePathSettings(FileName);
    createNewGraphWithNewFormat(FileName);
    show();
}

void GraphWindow::saveGraphWithNewFormat() // Сохранение файла с форматированием (цвет, позиция, значения вершин и пропускных способностей)
{
    stopAnim();

    QString a = graphToTextParse();
    if(a.size() == 0)
        return;

    QString FileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"),readPathSettings(),
                                                    tr("ZGraph File(*.zgrph)"));
    if (FileName.isEmpty())
        return;

    QFile File(FileName);
    if(File.open(QIODevice::WriteOnly))
    {
        QByteArray Data;
        Data = QCompressor::cryptData(a);
        File.write(Data);
        File.close();
    }
}

bool GraphWindow::createNewGraph(QString temp)
{
    stopAnim();
    nodes.clear();
    child_of_nodes.clear();
    cleanGraph();
    temp  = temp.trimmed();
    QTextStream stream(&temp);
    while(!stream.atEnd())
    {
        QString line = add_spaces_and_simplifie(stream.readLine(220));
        if(line.isEmpty()) continue;

        QStringList list = line.split(":");
        if(list.first() == list.last()) return false;

        QString t = list.first().trimmed();
        nodes << t;

        QString t2 = list.last().trimmed();
        QStringList list_of_children = t2.split(" , ",QString::SkipEmptyParts);

        child_of_nodes << list_of_children;
    }
    ui->main_graph->getMscene()->createGraphWithText(nodes,child_of_nodes);
    return true;
}

bool GraphWindow::createNewGraphWithFormat(QString file_name)
{ // Форматированный граф

    stopAnim();
    color_of_nodes.clear();
    values_of_edges.clear();
    points_of_nodes.clear();
    QString temp;
    QFile File(file_name);
    if(File.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        temp = File.readAll();
        File.close();
    }
     else
        return false;

    QString for_next = textToGraphParse(temp);
    if(!createNewGraph(for_next))
        return false;
    setFormat();
    return true;

}

bool GraphWindow::createNewGraphWithNewFormat(QString file_name)
{
    stopAnim();//форматированный граф
    color_of_nodes.clear();
    values_of_edges.clear();
    points_of_nodes.clear();
    QByteArray temp1;

    QFile File(file_name);
    if(File.open(QIODevice::ReadOnly))
    {
        temp1 = File.readAll();
        File.close();
    }
     else
        return false;
    QString temp = QCompressor::decryptData(temp1);
    QString for_next = textToGraphParse(temp);
    if(!createNewGraph(for_next))
        return false;
    setFormat();
    return true;

}

MScene* GraphWindow::getMscene(){
    return ui->main_graph->getMscene();
}

void GraphWindow::cleanGraph() // Очистить граф
{
    // Удалить все вершины (все связанные ребра удалятся тоже)
    foreach (Node* node, *(getMscene()->nodesList()))
        node->remove();
}

// Установить форматирование
void GraphWindow::setFormat()
{
	for (int i = 0; i < nodes.size(); i++)
	{
		Node * node = getMscene()->nodesList()->at(i);
		node->setColor(color_of_nodes[i]);
		node->setPos(points_of_nodes[i]);
        int k = 0;
        foreach (Node * ch, *node->getChildren()) {
			Edge * e = getMscene()->findEdge(node,ch);
			if(e)
				e->setValue(values_of_edges[i].at(k).toInt());
			k++;
		}
		node->update();
	}
}

QString GraphWindow::add_spaces_and_simplifie(QString str_for_work)
{
	QChar symbs[2]{':',','};
	for(int i = 0; i < 2; i++)
	{
		int a = -2;
		while((a = str_for_work.indexOf(symbs[i],a+2))!=-1)
		{
			{
				str_for_work.insert(a, ' ');
				str_for_work.insert(a+2, ' ');
			}
		}
	}
	str_for_work = str_for_work.simplified();
	return str_for_work;
}

void GraphWindow::showCreateHints()
{
    InformationDialog* brows = new InformationDialog(":/help","1.htm");
	brows->show();
}

void GraphWindow::showTokHints()
{
    QDialog* inf  = new QDialog(this);
    inf->setWindowModality(Qt::ApplicationModal);
    QHBoxLayout * lay = new QHBoxLayout;
    QLabel* e = new QLabel;
    e->setFixedSize(180,180);

    QMovie * mov = new QMovie(":help/me.jpg");
    mov->setScaledSize(QSize(180,180));
    e->setMovie(mov);
    mov->start();

    QLabel * c = new QLabel("<center><big>Студент: Ковынев М.В.<p></p>Группа: 6304 - ПИ<p></p>Почта: kovinevmv@gmail.com</center></big>");
    c->setWordWrap(true);

    lay->addWidget(e);
    lay->addWidget(c);

    QPalette pal;
    pal.setColor(QPalette::Window,Qt::white);
    inf->setPalette(pal);

    inf->setFixedSize(600,200);
    inf->setWindowTitle("Об авторе");
    inf->setLayout(lay);

    inf->show();
    return;
}

void GraphWindow::convertToUnoriented()
{
    // Создать ребро в другую сторону
	foreach (Node * node, *getMscene()->nodesList()) {
        foreach(Node * child_node,*node->getChildren())
            if(!Node::isNodesConnected(child_node,node))
				getMscene()->addItem(new Edge(child_node, node,getMscene()));
	}
}

void GraphWindow::clearFormat()
{
    // Очистить форматирование (цвета и стиль линий)
	foreach (QGraphicsItem * it, getMscene()->items()) {
			if(it->type() == Node::Type)
                ((Node*)it)->setColor(QColor(255,235,0));

			if(it->type() == Edge::Type)
			{
				((Edge*)it)->setColor(Qt::black);
				((Edge*)it)->setStyle(Qt::SolidLine);
				((Edge*)it)->setZValue(-1);
			}
	}
}

void GraphWindow::closeEvent(QCloseEvent * event)
{
	if (processing)
	{
        // Если работает алгоритм - спросить, завершить ли его
		int b = QMessageBox::question(this,"?","Алгоритм не завершил работу. Завершить принудительно?",QMessageBox::Yes,QMessageBox::No);
		if(b == QMessageBox::Yes)
		{
			processing = false;
			paused = false;
            msec_delay = 1;
		}
		event->ignore();
		return;
	}
    // Если алгоритм не работает - очистить анимацию и дать время на удаление.
	hide();
	stopAnim();
	writeSettings();
	MScene::setDelay(1000);

	event->accept();
}

void GraphWindow::maxFlowInit()
{
    stopAnim(); // Остановка всей анимации

	size = getMscene()->nodesList()->size();
	if(size<=1)
		return;

	int st = getMscene()->nodesList()->indexOf(getMscene()->findNode("S"));
    if(st == -1) // Нет истока
	{
		QMessageBox::information(this,"!","Нет обозначенного истока S",QMessageBox::Ok);
		return;
	}

	int fin = getMscene()->nodesList()->indexOf(getMscene()->findNode("T"));
    if(fin == -1) // Нет стока
	{
		QMessageBox::information(this,"!","Нет обозначенного стока T",QMessageBox::Ok);
		return;
	}

	(*getMscene()->nodesList())[st]->setColor(QColor(180,0,0));
	(*getMscene()->nodesList())[fin]->setColor(QColor(240,0,0));

    ui->gridLayout->menuBar()->setDisabled(true); // Отключить меню
    msec_delay = 600;    // Задержка на стандартные 6800 мсек.
    processing = true; //Алгоритм работает
	paused = true;

    //в Виджет для управления алгоритмом
	QWidget * setup_widget = new QWidget(this);

    setup_widget->setAttribute(Qt::WA_DeleteOnClose);
    setup_widget->setWindowModality(Qt::NonModal);
    setup_widget->setGeometry(28, 55, 200, 80);

     QPushButton * pause_play_but = new QPushButton(setup_widget);
     pause_play_but->setIcon(QIcon(":/player/play.png"));
     pause_play_but->setCheckable(true);

     QPushButton * one_step_but = new QPushButton(setup_widget);
     one_step_but->setIcon(QIcon(":/player/forward.png"));
     one_step_but->setToolTip("Следующий шаг");

     connect(one_step_but,&QPushButton::clicked,[=]() { msec_delay = 1; paused = false;
          QTimer::singleShot(1,[=](){paused = true; msec_delay = 5000;}); });

     connect(pause_play_but,&QPushButton::toggled,[=](bool ch) {paused = !paused;
          one_step_but->setEnabled(ch ? 0 : 1); pause_play_but->setIcon(ch ? QIcon(":/player/pause.png")
           : QIcon(":/player/play.png")); pause_play_but->setToolTip(!ch ?  "Продолжить" :"Остановить"); });

      QLabel* head = new QLabel;
      head->setText("<center>Параметры воспроизведения</center>");

      QLabel* play = new QLabel;
      play->setText("<center>Запустить поиск</center>");

      QLabel* step = new QLabel;
      step->setText("<center>След. шаг</center>");

      QGridLayout *vbox = new QGridLayout(setup_widget);
      vbox->addWidget(head, 0, 0, 1, 2);
      vbox->addWidget(play, 1, 0);
      vbox->addWidget(step, 1, 1);
      vbox->addWidget(pause_play_but, 2, 0);
      vbox->addWidget(one_step_but, 2, 1);
      vbox->setMargin(1);

    setup_widget->setLayout(vbox);
    setup_widget->show();

	capacity = new int *[size];
	for(int i = 0; i < size; i++)
		capacity[i] = new int[size];

	flow = new int *[size];
	for(int i = 0; i < size; i++)
		flow[i] = new int[size];

	for(int i =0; i < size; i++)
	{
		for(int j =0 ; j < size; j++)
		{
			capacity[i][j] = 0;
			flow[i][j] = 0;
		}
	}

	pred = new int[size];

    // Заполнение матрицы пропускных способностей
    QString matrixString = "Исходная матрица:\n";
    foreach (Node* n, *getMscene()->nodesList())
    {
        matrixString += "[";
		int a = getMscene()->nodesList()->indexOf(n);
        foreach (Node* z, *getMscene()->nodesList())
        {
			int b = getMscene()->nodesList()->indexOf(z);
			Edge* e = getMscene()->findEdge(n,z);
			if(e)
				capacity[a][b] = e->getValue();

            matrixString+= QString::number(capacity[a][b]) + " ";
		}
        matrixString += "]\n";

	}

    int m_flow= max_flow(st,fin); // Вычисление макс. потока

    if(processing) // Если алгоритм не был прерван
	{
		QMessageBox::information(this,"Максимальный поток","Максимальный поток в сток Т - " + QString::number(m_flow),QMessageBox::Ok);
		processing = false;
		foreach (QGraphicsItem * it, getMscene()->items()) {
			if(it->type() == Edge::Type)
			{
				Edge* tmp = ((Edge*)it);
                if(tmp->getCurrent() == 0) // Если через ребро нет потока
                {	// Окрасить в серый
					tmp->setColor(Qt::lightGray);
					tmp->setZValue(-4);
				}
			}
		}

	}
    else // Если алгоритм прерван
	{
			QMessageBox::information(this,"!","Сток в вершину T не найден",QMessageBox::Ok);
			stopAnim();
	}
	setup_widget->close();

	ui->gridLayout->menuBar()->setDisabled(false);

    matrixString += log;
    matrixString += "\nПреобразованная матрица\n";
	for(int i = 0; i < size; i++)
    {
        matrixString += "[";
        for (int j = 0; j < size; j++)
            matrixString+= QString::number(flow[i][j]) + " ";

        matrixString += "]\n";
    }

    showTextGraph(matrixString);


    for(int i = 0; i < size; i++)
        delete capacity[i];
	delete capacity;

	for(int i = 0; i < size; i++)
		delete flow[i];
	delete flow;

	delete pred;
}

bool GraphWindow::bfs(int start,int end)
{
	QVector<bool> is_visited(size);
	QVector<int> qq;

	qq.push_back(start);
	is_visited[start] = true;
	pred[start] = -1;   // Специальная метка для начала пути

	while(!qq.isEmpty() && processing)  // Пока хвост не совпадёт с головой
	{
        // Вершина u пройдена
		int u = qq.first();
		qq.pop_front();
		is_visited[u] = true;
		for(int v = 0; v < size; v++ ) // Смотрим смежные вершины
		{
			// Если не пройдена и не заполнена
			if(!is_visited[v] && capacity[u][v] > flow[u][v]) {
				qq.push_back(v);
				is_visited[v] = true;
				pred[v] = u; // Путь обновляем
			}
		}
	}

	if(is_visited[end]) // Если конечная вершина, дошли - возвращаем 0
	  return true;
	else
		return false;
}

int GraphWindow::max_flow(int source, int stock)
{

    log.clear();
    log += "\nНачало алгоритма:\n-----------------------------------";
    int maxflow=0;            // Изначально нулевой
    while(bfs(source,stock) && processing)  // Пока существует путь
    {
        int delta = 10000000;
        for(int u = stock; pred[u] >= 0 && processing; u = pred[u]) // Найти минимальный поток в пути
            delta=qMin(delta, ( capacity[pred[u]][u] - flow[pred[u]][u] ) );


        for(int u = stock; pred[u] >= 0 && processing; u=pred[u]) // По алгоритму Форда-Фалкерсона
        {
            Node * a = (*getMscene()->nodesList())[pred[u]];
            Node * b = (*getMscene()->nodesList())[u];
            Edge * e = getMscene()->findEdge(a,b);

            log += "\nОбходим вершины: " + a->getValue() + " и "+  b->getValue();
            log += "\nПредыдущее значение: " + QString::number(e->getCurrent());
            log += "\nИзменение потока: " + QString::number(delta);

            flow[pred[u]][u] += delta;
            flow[u][pred[u]] -= delta;

            {   //Графическое отображение

                // Окрасить задействованные вершины
                if(pred[u] != source && pred[u] != stock)
                        a->setColor(QColor(255,50,50));
                  if(u != source && u != stock)
                        b->setColor(QColor(255,50,50));

                  e->setColor(QColor(180,0,0)); // Окрасить ребро
                  if(!e->isAnimating())         // Анимировать
                      e->setAnimating(true);
                  e->setSelected(true);         // Выделить (жирным)

                if(!e->setCurrent(flow[pred[u]][u])) //прервать алгоритм
                    processing = false;				 //при отриц. потоке

                log += "\nНовое значение: ";
                log += QString::number(e->getCurrent());
                log += "\n-----------------------------------";

                while(paused) // Пауза
                    QApplication::processEvents();

                    MScene::setDelay(msec_delay); // Задержка между шагами


                e->setSelected(false);  //снять выделение
                e->setColor(Qt::black); //вернуть цвет ребру

                if(pred[u] != source && pred[u] != stock) //вернуть цвет вершинам
                        a->setColor(QColor(255,235,0));
                if(u != source && u != stock)
                        b->setColor(QColor(255,235,0));
            }
        }
        maxflow += delta;  // Повышаем максимальный поток
    }

    log += "\nКонец алгоритма\n\n";
    return maxflow;

}

void GraphWindow::writePathSettings(QString str)
{
	QFileInfo inf(str);
	QString b = inf.absoluteDir().absolutePath();
	QSettings settings("LETI", "Graph application");
	settings.setValue("path",b);
}

QString GraphWindow::readPathSettings()
{
	QSettings settings("LETI", "Graph application");
	QString str = settings.value("path").toString();
	return str;
}

void GraphWindow::writeSettings()
{
	QSettings settings("LETI", "Graph application");
	settings.setValue("geometry",saveGeometry());
}

//установка настроек
void GraphWindow::readSettings()
{
	QSettings settings("LETI", "Graph application");
	restoreGeometry(settings.value("geometry").toByteArray());
}

void GraphWindow::startSearchMaxFlow()
{
    stopAnim();
    maxFlowInit();
}

void GraphWindow::stopAnim()
{
    foreach (QGraphicsItem * it, getMscene()->items())
    {
        if(it->type() == Edge::Type)
        {
            // Для каждого ребра остановить анимацию и обнулить текущий поток
            ((Edge*)it)->setAnimating(false);
            ((Edge*)it)->setCurrent(-1);
        }
    }
    clearFormat();
}

void GraphWindow::stopAnimationButton()
{
    stopAnim();
}

bool GraphWindow::createRandomGraph()
{
    stopAnim();
    srand(time(NULL));
    QString x = "";

    int size = rand() % 10 + 3;

    for (int i = 0; i < size; i++)
    {
        x+= QString::number(i) + " : " + QString::number(rand() % size) + " &%& "+
                QString::number(rand()%600-300) + " " + QString::number(rand()%600-300) + " &%& " +
                QString::number(rand()%255) + " " +  QString::number(rand()%255) + " " +
                QString::number(rand()%255) + " &%& " +  QString::number(rand()%20 + 1) + "\n";
    }

    QString for_next = textToGraphParse(x);

    if(!createNewGraph(for_next))
        return false;
    setFormat();
    return true;
}

bool GraphWindow::example1Graph()
{
    stopAnim();
    cleanGraph();

    QString x = "d : a, b, T &%& 241 190 &%& 255 235 0 &%& 4 6 3 \nc : a, "
                "d &%& -177 188 &%& 255 235 0 &%& 1 7 \nS : a, c &%& -516 5"
                " &%& 180 0 0 &%& 5 9 \na : b &%& -171 -206 &%& 255 235 0 &%&"
                " 7 \nb : T &%& 240 -204 &%& 255 235 0 &%& 5 \nT :  &%& 544 1"
                " &%& 240 0 0 &%& \n";

    QString for_next = textToGraphParse(x);

    if(!createNewGraph(for_next))
        return false;
    setFormat();
    return true;
}

bool GraphWindow::example2Graph()
{
    stopAnim();
    cleanGraph();

    QString  x = "S : 1, 2 &%& -775 -268 &%& 255 255 255 &%& 11 13 \n1 : 3, 2 &%& -511 -490"
               " &%& 255 255 255 &%& 12 10 \n2 : 1, 4 &%& -473 -152 &%& 255 255 255 &%& 4 "
               "14 \n3 : T, 2 &%& -305 -396 &%& 255 255 255 &%& 20 9 \n4 : T, 3 &%& -222 -106"
               " &%& 255 255 255 &%& 4 7 \nT :  &%& -114 -282 &%& 255 255 255 &%& \n";

    QString for_next = textToGraphParse(x);

    if(!createNewGraph(for_next))
        return false;
    setFormat();
    return true;
}

QString GraphWindow::textToGraphParse(QString x)
{
    color_of_nodes.clear();
    values_of_edges.clear();
    points_of_nodes.clear();

    QString temp = x;
    QString for_next;

    QTextStream stream(&temp);
    qreal a,b, c;
    while(!stream.atEnd())
    {
        QString temp2 =add_spaces_and_simplifie( stream.readLine(220));
        if (temp2.isEmpty())
            continue;
        QStringList list1 = temp2.split("&%&");
        for_next.append(list1.first().trimmed() + "\n");

        QTextStream temp3(&list1[1]);
        temp3 >> a >> b;
        points_of_nodes << QPointF(a,b);

        temp3.setString(&list1[2]);
        temp3 >> a >> b >> c;
        color_of_nodes << QColor(a,b,c);

        QString ee = list1[3].trimmed();
        values_of_edges << ee.split(" ");
        QString ss;
        foreach (QString a, values_of_edges.back()) {
            ss.append(a + " ");
        }

    }

    return for_next;
}

QString GraphWindow::graphToTextParse()
{
    QString a;
    // Заполнение строки основной информацией
    foreach (Node* node, *(ui->main_graph->getMscene()->nodesList())){
        a << *node;
        a.append(" &%& " + QString::number(node->scenePos().x()) +
                 " " + QString::number(node->scenePos().y())
                 +" &%& " + QString::number(node->getColor().red()) +
                 " " + QString::number(node->getColor().green()) +
                 " " + QString::number(node->getColor().blue())

                 +" &%& ");
        foreach(Node* ch,*node->getChildren())
        {
            Edge * e = getMscene()->findEdge(node,ch);
            a.append(e ? QString::number(e->getValue()) + " " : "0 ");

        }
        a.append("\n");
    }

    return a;
}
