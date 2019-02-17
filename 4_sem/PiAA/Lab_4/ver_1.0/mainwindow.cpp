#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_options.h"
#include "mscene.h"
#include "node.h"
#include "chose.h"
#include "edge.h"
#include "options.h"

#include <ctime>
#include "qcompressor.h"
#include <queue>
#include <QFile>
#include <QList>
#include <QMap>
#include <QQueue>
#include <QFileDialog>
#include <QSet>
#include <QMessageBox>
#include <helpbrowser.h>
#include <QEventLoop>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString add_spaces_and_simplifie(QString str_for_work)
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


void MainWindow::on_action_7_triggered()
{
    foreach (Node* node, *(ui->graphicsView->getMscene()->nodesList()))
        node->removeNode();
}


QByteArray cryptData(QString a)
{
    QString x;
    QString key = ";";
    for (int i=0; i < a.size(); i++)
    {
        char aa = a.toStdString().c_str()[i];
        char kk = QString(key).toStdString().c_str()[i % key.size()];
        x[i] = aa ^ kk;
    }


    QByteArray compressed;
    QCompressor::gzipCompress(x.toLatin1(), compressed);

    return compressed;
}



QString decryptData(QByteArray temp)
{

   QByteArray decompressed;
   QCompressor::gzipDecompress(temp, decompressed);
   QString x = QString::fromLatin1(decompressed);


    QString key = ";";
    for (int i=0; i < x.size(); i++)
    {
        char aa = x.toStdString().c_str()[i];
        char kk = QString(key).toStdString().c_str()[i % key.size()];
        x[i] = aa ^ kk;
    }
   return x;

}


void MainWindow::on_action_8_triggered()
{
    Options* opt = new Options(this);
    opt->ptr = this;
    opt->ui->horizontalSlider->setValue(opt->ptr->ui->graphicsView->getMscene()->diametr());
    opt->ui->horizontalSlider_2->setValue(opt->ptr->ui->graphicsView->getMscene()->arrSize());

    if(ui->graphicsView->getMscene()->shouldShowValues())
    {
        opt->ui->weight_graph_yes->setChecked(true);
        opt->ui->weight_graph_no->setChecked(false);
    }
    else
    {
        opt->ui->weight_graph_yes->setChecked(false);
        opt->ui->weight_graph_no->setChecked(true);
    }

    if(ui->graphicsView->getMscene()->isOriented())
    {
        opt->ui->or_graph_yes->setChecked(true);
        opt->ui->or_graph_no->setChecked(false);
    }
    else
    {
        opt->ui->or_graph_yes->setChecked(false);
        opt->ui->or_graph_no->setChecked(true);
    }
    opt->show();
}

void MainWindow::on_action_2_triggered()
{
    QDialog* inf  = new QDialog(this);
           inf->setWindowModality(Qt::ApplicationModal);
           QHBoxLayout * lay = new QHBoxLayout;
           QLabel* e = new QLabel;
           e->setFixedSize(180,180);

           QMovie * mov = new QMovie(":help/help/me.jpg");
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

void MainWindow::on_action_triggered()
{
    HelpBrowser brows(":/help/help","1.htm");
        brows.exec();
}


bool operator<(const std::pair<Node*, unsigned>& lhs, const std::pair<Node*, unsigned>& rhs)
{
    return lhs.second > rhs.second;
}

void MainWindow::on_action_1_triggered()
{    
    foreach (Node* node, *(ui->graphicsView->getMscene()->nodesList()))
        node->removeNode();

    ui->graphicsView->getMscene()->setOrientated(false);
    ui->graphicsView->getMscene()->setShowEdge(false);

    ui->graphicsView->getMscene()->addNode(QPointF(-130, -80), "0");
    ui->graphicsView->getMscene()->addNode(QPointF(-130, 80), "1");
    ui->graphicsView->getMscene()->addNode(QPointF(130, -80), "2");
    ui->graphicsView->getMscene()->addNode(QPointF(130, 80), "3");
    ui->graphicsView->getMscene()->addNode(QPointF(0, -130), "4");

    Node* node0 = ui->graphicsView->getMscene()->findNode("0");
    Node* node1 = ui->graphicsView->getMscene()->findNode("1");
    Node* node2 = ui->graphicsView->getMscene()->findNode("2");
    Node* node3 = ui->graphicsView->getMscene()->findNode("3");
    Node* node4 = ui->graphicsView->getMscene()->findNode("4");

    ui->graphicsView->getMscene()->addEdge(node0, node1);
    ui->graphicsView->getMscene()->addEdge(node0, node2);
    ui->graphicsView->getMscene()->addEdge(node0, node3);
    ui->graphicsView->getMscene()->addEdge(node0, node4);
    ui->graphicsView->getMscene()->addEdge(node1, node2);
    ui->graphicsView->getMscene()->addEdge(node1, node3);
    ui->graphicsView->getMscene()->addEdge(node1, node4);
    ui->graphicsView->getMscene()->addEdge(node2, node4);
    ui->graphicsView->getMscene()->addEdge(node3, node2);
    ui->graphicsView->getMscene()->addEdge(node3, node4);
}

void MainWindow::on_action_13_triggered()
{
    foreach (Node* node, *(ui->graphicsView->getMscene()->nodesList()))
        node->removeNode();

    ui->graphicsView->getMscene()->setOrientated(true);
    ui->graphicsView->getMscene()->setShowEdge(true);

    ui->graphicsView->getMscene()->addNode(QPointF(120, 95), "0");
    ui->graphicsView->getMscene()->addNode(QPointF(-120, 95), "1");

    ui->graphicsView->getMscene()->addNode(QPointF(-240, 0), "2");

    ui->graphicsView->getMscene()->addNode(QPointF(-120, -95), "3");

    ui->graphicsView->getMscene()->addNode(QPointF(120, -95), "4");

    Node* node0 = ui->graphicsView->getMscene()->findNode("0");
    Node* node1 = ui->graphicsView->getMscene()->findNode("1");
    Node* node2 = ui->graphicsView->getMscene()->findNode("2");
    Node* node3 = ui->graphicsView->getMscene()->findNode("3");
    Node* node4 = ui->graphicsView->getMscene()->findNode("4");

    ui->graphicsView->getMscene()->addEdge(node1, node2, 2);
    ui->graphicsView->getMscene()->addEdge(node0, node1, 12);
    ui->graphicsView->getMscene()->addEdge(node0, node2, 4);
    ui->graphicsView->getMscene()->addEdge(node0, node3, 10);
    ui->graphicsView->getMscene()->addEdge(node0, node4, 25);
    ui->graphicsView->getMscene()->addEdge(node2, node3, 3);
    ui->graphicsView->getMscene()->addEdge(node3, node4, 5);
}

#define WHITE 0
#define GREY 1
#define BLACK 2

void MainWindow::enque(int x)
{
  q[tail] = x;     // записать х в хвост
  tail++;          // хвостом становиться следующий элемент
  color[x] = GREY; // Цвет серый (из алгоритма поиска)
}

int MainWindow::deque()
{
  int x = q[head];  // Записать в х значение головы
  head++;           // Соответственно номер начала очереди увеличивается
  color[x] = BLACK; // Вершина х - отмечается как прочитанная
  return x;         // Возвращается номер х прочитанной вершины
}

int MainWindow::bfs(int start,int end)
{
    int u,v;
    for( u = 0; u < n; u++ ) // Сначала отмечаем все вершины не пройденными
      color[u]=WHITE;

    head=0;   // Начало очереди 0
    tail=0;   // Хвост 0
    enque(start);      // Вступили на первую вершину
    pred[start]= -1;   // Специальная метка для начала пути
    while(head!=tail && processing )  // Пока хвост не совпадёт с головой
    {
      u=deque();       // вершина u пройдена
      for( v = 0; v < n; v++ ) // Смотрим смежные вершины
      {
       // Если не пройдена и не заполнена
       if(color[v] == WHITE && (capacity[u][v]-flow[u][v]) > 0) {
         enque(v);  // Вступаем на вершину v
         pred[v]=u; // Путь обновляем
       }
      }
    }
    if(color[end] == BLACK) // Если конечная вершина, дошли - возвращаем 0
      return 0;
    else return 1;
}

int MainWindow::max_flow(int source, int stock)
{
    int maxflow=0;            // Изначально нулевой
    while(bfs(source,stock) == 0)             // Пока сеществует путь
      {
        int delta=10000;
        for(int u = n-1; pred[u] >= 0 && processing ; u=pred[u]) // Найти минимальный поток в сети
        {
          delta=qMin(delta, ( capacity[pred[u]][u] - flow[pred[u]][u] ) );
        }
        for(int u = n-1; pred[u] >= 0 && processing ; u=pred[u]) // По алгоритму Форда-Фалкерсона
        {
              Node* a = (*ui->graphicsView->getMscene()->nodesList())[pred[u]];
              Node* b = (*ui->graphicsView->getMscene()->nodesList())[u];
              Edge* e = ui->graphicsView->getMscene()->findEdge(a,b);

              if(pred[u] != 0 && pred[u] != n-1)
                        a->setColor(QColor(255,50,50));
              if(u != 0 && u != n-1)
                        b->setColor(QColor(255,50,50));

              flow[pred[u]][u] += delta;
              flow[u][pred[u]] -= delta;
              e->setColor(QColor(180,0,0));

              if (flow[pred[u]][u] < 0){
                  processing = false;
                  e->setValue(flow[pred[u]][u]);
              }
              else{
                  e->setValue(flow[pred[u]][u]);
              }

              QEventLoop loop;
              QTimer timer;
              timer.setInterval(1500);
              connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
              timer.start();
              loop.exec();

              e->setColor(Qt::black);
              if(pred[u] != 0 && pred[u] != n-1)
                    a->setColor(Qt::yellow);
              if(u != 0 && u != n-1)
                    b->setColor(Qt::yellow);
        }
        maxflow+=delta;                       // Повышаем максимальный поток
      }
    return maxflow;
}

void MainWindow::on_action_14_triggered()
{
    foreach (Node* node, *(ui->graphicsView->getMscene()->nodesList()))
        node->removeNode();

    ui->graphicsView->getMscene()->setOrientated(true);
    ui->graphicsView->getMscene()->setShowEdge(true);

    ui->graphicsView->getMscene()->addNode(QPointF(100, 95), "d");
    ui->graphicsView->getMscene()->addNode(QPointF(-120, 95), "c");
    ui->graphicsView->getMscene()->addNode(QPointF(-240, 0), "S");
    ui->graphicsView->getMscene()->addNode(QPointF(-120, -95), "a");
    ui->graphicsView->getMscene()->addNode(QPointF(100, -95), "b");
    ui->graphicsView->getMscene()->addNode(QPointF(240, 0), "T");

    Node* nodeS = ui->graphicsView->getMscene()->findNode("S");
    Node* nodeT = ui->graphicsView->getMscene()->findNode("T");
    Node* nodea = ui->graphicsView->getMscene()->findNode("a");
    Node* nodeb = ui->graphicsView->getMscene()->findNode("b");
    Node* nodec = ui->graphicsView->getMscene()->findNode("c");
    Node* noded = ui->graphicsView->getMscene()->findNode("d");

    ui->graphicsView->getMscene()->addEdge(nodeS, nodea, 5);
    ui->graphicsView->getMscene()->addEdge(nodeS, nodec, 9);
    ui->graphicsView->getMscene()->addEdge(nodec, nodea, 1);
    ui->graphicsView->getMscene()->addEdge(nodec, noded, 7);
    ui->graphicsView->getMscene()->addEdge(noded, nodea, 4);
    ui->graphicsView->getMscene()->addEdge(nodea, nodeb, 7);
    ui->graphicsView->getMscene()->addEdge(noded, nodeb, 6);
    ui->graphicsView->getMscene()->addEdge(nodeS, nodea, 5);
    ui->graphicsView->getMscene()->addEdge(nodeb, nodeT, 5);
    ui->graphicsView->getMscene()->addEdge(noded, nodeT, 3);
}

void MainWindow::on_action_15_triggered()
{
    QApplication::quit();
}

void MainWindow::on_pushButton_clicked()
{
    if(!ui->graphicsView->getMscene()->isOriented()
            || !ui->graphicsView->getMscene()->shouldShowValues())
    {
        QMessageBox::warning(this, "Ошибка", "Для поиска граф должен быть оринтированным и должны"
                                             " отображаться веса");
        return;
    }

    if(ui->graphicsView->getMscene()->nodesList()->size() < 2)
    {
        QMessageBox::warning(this, "Ошибка", "Для данного алгоритма нужно более 1 вершины в графе");
        return;
    }

    chose* choose = new chose(this);
    choose->setFixedSize(174, 185);
    QString start_name;
    QString finish_name;
    choose->start_name = &start_name;
    choose->finish_name = &finish_name;
    choose->exec();

    if (start_name ==  "\0"
            && finish_name == "\0")
        return;

    Node* start = ui->graphicsView->getMscene()->findNode(start_name);
    Node* dest = ui->graphicsView->getMscene()->findNode(finish_name);

    if (!start || !dest)
    {

        QMessageBox::warning(this, "Ошибка", "Нет таких вершин");
        return;
    }

    setEnabled(false);
    start->setColor(QColor(255,0,0));
    dest->setColor(QColor(255,0,0));

    int size = ui->graphicsView->getMscene()->nodesList()->size();
    int st = ui->graphicsView->getMscene()->nodesList()->indexOf(start);
    if(st!=0)
        qSwap((*ui->graphicsView->getMscene()->nodesList())[st], ui->graphicsView->getMscene()->nodesList()->first());
    int fin = ui->graphicsView->getMscene()->nodesList()->indexOf(dest);
    if(fin!=size-1)
        qSwap((*ui->graphicsView->getMscene()->nodesList())[fin], ui->graphicsView->getMscene()->nodesList()->last());

    processing = true;
    capacity = new int*[size];
    for(int i = 0; i < size; i++)
        capacity[i] = new int[size];

    flow = new int*[size];
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

    color = new int[size+5];
    pred = new int[size+5];
    q = new int[size+5];
    n = size;

    foreach (Node* n, *ui->graphicsView->getMscene()->nodesList()) {
        int a = ui->graphicsView->getMscene()->nodesList()->indexOf(n);
        foreach (Node* z, *ui->graphicsView->getMscene()->nodesList()) {
            int b = ui->graphicsView->getMscene()->nodesList()->indexOf(z);
            Edge* e = ui->graphicsView->getMscene()->findEdge(n,z);
            if(e)
                capacity[a][b] = e->getValue();
            }
        }

    int m_flow= max_flow(0,n-1);
    if(processing){
    QMessageBox::information(this,"Максимальный поток",
                             "Максимальный поток в сток " + QString::number(m_flow),QMessageBox::Ok);
    processing = false;
    foreach (QGraphicsItem * it, ui->graphicsView->getMscene()->items()) {
        if(it->type() == Edge::Type)
        {
            Edge* tmp = ((Edge*)it);
            if(tmp->get_flow_value() == 0)
                {
                tmp->setColor(QColor(Qt::gray).lighter(110));
                }
            }
        }
    }
    else{
        QMessageBox::information(this,"!","Сток в вершину T не найден",QMessageBox::Ok);
    }
    for(int i = 0; i < size; i++)
        delete capacity[i];
    delete capacity;

    for(int i = 0; i < size; i++)
        delete flow[i];
    delete flow;

    delete color;
    delete pred;
    delete q;

    setEnabled(true);
}

void MainWindow::on_action_19_triggered()
{
    QString a;
    foreach (Node* node, *(ui->graphicsView->getMscene()->nodesList())){
        a << *node;
        a.append("\n");}

    if(a.size() == 0)
        return;

    QString FileName = QFileDialog::getSaveFileName(this, tr("Сохранить в файл"), QString(),
                                                    tr("Text File(*.txt)"));
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

void MainWindow::on_action_20_triggered()
{
    QString FileName = QFileDialog::getOpenFileName(this, "Открыть файл", QString(), "Text files(*.txt)");
    if (FileName.isEmpty())
        return;

    QFile File(FileName);
    QString temp;

    if(File.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        temp = File.readAll();
        File.close();
    }




    QList<QString> nodes;
    QList<QStringList> child_of_nodes;
    temp  = temp.trimmed();
    QTextStream stream(&temp);

    foreach (Node* node, *(ui->graphicsView->getMscene()->nodesList()))
        node->removeNode();

    while(!stream.atEnd())
    {
        QString line = add_spaces_and_simplifie(stream.readLine(120));
        if(line.isEmpty()) continue;

        QStringList list = line.split(":");

        if(list.first() == list.last())
            return;

        QString t = list.first().trimmed();
        nodes << t;

        QString t2 = list.last().trimmed();
        QStringList list_of_children = t2.split(" , ",QString::SkipEmptyParts);

        child_of_nodes << list_of_children;
    }
    ui->graphicsView->getMscene()->createGraphWithText(nodes,child_of_nodes);
}

void MainWindow::openFile(QString temp)
{

    QTextStream stream(&temp);
    QList<QColor> color_of_nodes;
    QList<QPointF> points_of_nodes;
    QList<QStringList> values_of_edges;
    QString for_next;

    QString orientation = stream.readLine().trimmed();
    qreal a,b,c;

    foreach (Node* node, *(ui->graphicsView->getMscene()->nodesList()))
        node->removeNode();

    while(!stream.atEnd())
    {
        QString temp2 =add_spaces_and_simplifie(stream.readLine(220));
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

    QList<QString> nodes;
    QList<QStringList> child_of_nodes;

    foreach (Node* node, *(ui->graphicsView->getMscene()->nodesList()))
        node->removeNode();


    for_next = for_next.trimmed();
    QTextStream stream_(&for_next);

    while(!stream_.atEnd())
    {
        QString line = add_spaces_and_simplifie(stream_.readLine(220));
        if(line.isEmpty()) continue;

        QStringList list = line.split(":");
        if(list.first() == list.last())
            return;

        QString t = list.first().trimmed();
        nodes << t;

        QString t2 = list.last().trimmed();
        QStringList list_of_children = t2.split(" , ",QString::SkipEmptyParts);

        child_of_nodes << list_of_children;
    }

    ui->graphicsView->getMscene()->createGraphWithText(nodes,child_of_nodes);

    for (int i = 0; i < nodes.size(); i++)
    {
        Node * node = ui->graphicsView->getMscene()->nodesList()->at(i);
        node->setColor(color_of_nodes[i]);
        node->setPos(points_of_nodes[i]);
        int k =0;
        foreach (Node * ch, *node->children()) {
            Edge * e = ui->graphicsView->getMscene()->findEdge(node,ch);
            if(e)
                e->setValue(values_of_edges[i].at(k).toInt());
            k++;
        }
        node->update();
    }

    if( orientation == "nonoriented")
        ui->graphicsView->getMscene()->setOrientated(false);
    else
        ui->graphicsView->getMscene()->setOrientated(true);
}

void MainWindow::on_action_grph_3_triggered()
{
    QString FileName = QFileDialog::getOpenFileName(this, "Открыть файл",QString(), "Graph File(*.grph)");
    if (FileName.isEmpty())
        return;

    //QByteArray temp;
    QString temp;
    QFile File(FileName);
    if(File.open(QIODevice::ReadOnly))
    {
        temp = File.readAll();
        File.close();
    }

    else
       return;

   openFile(temp);

}

void MainWindow::on_action_3_triggered()
{
    QString FileName = QFileDialog::getOpenFileName(this, "Открыть файл",QString(), "Graph File(*.zgrph)");
    if (FileName.isEmpty())
        return;

    QByteArray temp;
    QFile File(FileName);
    if(File.open(QIODevice::ReadOnly))
    {
        temp = File.readAll();
        File.close();
    }
    else
       return;
    QString x = decryptData(temp);

    openFile(x);
}

QString MainWindow::saveFile()
{
    QString a;
    if(ui->graphicsView->getMscene()->isOriented())
        a.append("oriented\n");
    else
        a.append("nonoriented\n");

    foreach (Node* node, *(ui->graphicsView->getMscene()->nodesList())){
        a << *node;
        a.append(" &%& " + QString::number(node->scenePos().x()) + " " + QString::number(node->scenePos().y())

                 +" &%& " + QString::number(node->get_color().red()) +
                 " " + QString::number(node->get_color().green()) +
                 " " + QString::number(node->get_color().blue())

                 +" &%& ");
        foreach(Node* ch,*node->children())
        {
            Edge * e = ui->graphicsView->getMscene()->findEdge(node,ch);
            if(e)
                a.append(QString::number(e->getValue()) + " ");
            else
                a.append("0 ");
        }
        a.append("\n");
    }
    return a;
}

void MainWindow::on_action_21_triggered()
{
    QString a = saveFile();

    QString FileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"), QString(),
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

void MainWindow::on_action_zgrph_2_triggered()
{
    QString a = saveFile();

    QString FileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"), QString(),
                                                    tr("Graph File(*.zgrph)"));
    if (FileName.isEmpty())
        return;

    QFile File(FileName);
    if(File.open(QIODevice::WriteOnly))
    {
        QByteArray Data = cryptData(a);

        File.write(Data);
        File.close();
    }
}

void MainWindow::on_action_6_triggered()
{
    srand(time(NULL));
    QString x = "";
    x += "oriented\n";


   int size = rand() % 10 + 3;

    for (int i = 0; i < size; i++)
    {
        x+= QString::number(i) + " : " + QString::number(rand() % size) + " &%& "+
                QString::number(rand()%600-300) + " " + QString::number(rand()%600-300) + " &%& " +
                QString::number(rand()%255) + " " +  QString::number(rand()%255) + " " +
                QString::number(rand()%255) + " &%& " +  QString::number(rand()%20 + 1) + "\n";
    }

    qDebug() << x;
    openFile(x);
}




