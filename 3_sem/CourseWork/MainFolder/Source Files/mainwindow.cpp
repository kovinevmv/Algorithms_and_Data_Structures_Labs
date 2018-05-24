#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include "graphwidget.h"
#include "edge.h"
#include "node.h"
#include <math.h>
#include <vector>
#include <iostream>
#include <QProcess>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
using namespace std;


int numberTreeEl;
binTree b;
vector<int> values(numberTreeEl);



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit->setPlaceholderText
            ("Введите последовательность чисел для создания бинарного дерева");
    ui->pushButton->setEnabled(0);
    ui->comboBox->setEnabled(0);
    ui->textEdit_2->setEnabled(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}


bool isCreatedBinTreeRoot = false;
void GraphWidget::addElementsToScene(binTree b, QGraphicsScene *scene,
    Node* rootElement, bool isLeftElement, double x, double y, double ratioImmersion)
{
    Node* aa =new Node(this);

    if (!isCreatedBinTreeRoot)
    {
        centerNode =new Node(this);
        scene->addItem(centerNode);
        centerNode->setPos(x,y);
        isCreatedBinTreeRoot = true;;
        if (Left(b)!=NULL)
            addElementsToScene(Left(b), scene,  centerNode, 1, x, y, ratioImmersion);
        if (Right(b)!=NULL)
            addElementsToScene(Right(b), scene, centerNode, 0, x, y, ratioImmersion);
    }
    else
    {
        scene->addItem(aa);
        if (isLeftElement)
            x -=(50.0*ratioImmersion);
        else
            x +=(50.0*ratioImmersion);

        y +=50.0;
        aa->setPos(x,y);
        scene->addItem(new Edge(rootElement, aa));
        if (Left(b)!=NULL)
            addElementsToScene(Left(b), scene, aa, 1, x,y, ratioImmersion/1.75);
        if (Right(b)!=NULL)
            addElementsToScene(Right(b), scene, aa, 0, x,y, ratioImmersion/1.75);
    }

}


GraphWidget::GraphWidget(QWidget *parent) : QGraphicsView(parent), timerId(0)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-900, -400, 1800, 800);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(400, 400);
    setWindowTitle(tr("Курсовая работа"));

    addElementsToScene(b,scene, NULL, 0, 0.0, -350.0, 4);
}

int indexNameNode=0;
void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    QRadialGradient gradient(-3, -3, 10);
    QRectF textRect(10,0, 200,200);
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(16);

    gradient.setColorAt(0, QColor(255, 104, 0));
    gradient.setColorAt(1, QColor(200, 55, 0));

    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::darkRed, 0));
    painter->drawEllipse(-10, -10, 20, 20);

    painter->drawText(textRect, QString::
                      number(values[indexNameNode % numberTreeEl]));
    indexNameNode++;
}


int indexNameNode2=0;
void bypassBinTree(binTree b)
{
    values[indexNameNode2]=RootBT(b);
    indexNameNode2++;
    if (Left(b)!=NULL)
        bypassBinTree(Left(b));
    if (Right(b)!=NULL)
        bypassBinTree(Right(b));
}


void MainWindow::on_pushButton_clicked()
{
    QString string = ui->textEdit->toPlainText();
    QStringList list = string.split(QRegExp("\\s"), QString::SkipEmptyParts);

    int t=0;
    for (int i=0; i<list.size(); i++)
    {
        QString x;
        x=list[i];
        b = insertrandom(b, x.toInt(), t);
    }

    numberTreeEl = CountTreeEl(b);

    values.resize(numberTreeEl);
    bypassBinTree(b);

    GraphWidget *widget = new GraphWidget;
    widget->show();

    for (int i=0; i<list.size(); i++)
    {
        QString x;
        x=list[i];
        ui->comboBox->addItem(x);
    }

}

void MainWindow::on_textEdit_textChanged()
{

    ui->pushButton->setEnabled(1);
    ui->comboBox->setEnabled(1);
    ui->textEdit_2->setEnabled(1);

}

void MainWindow::on_pushButton_2_clicked()
{
    QProcess::startDetached(QApplication::applicationFilePath(),
        QStringList(), QApplication::applicationDirPath());
    exit(1);
}

void MainWindow::bsearchBinTree(binTree b, int x, QString path)
{
    if (x != RootBT(b))
    {
        ui->textEdit_2->setText(ui->textEdit_2->toPlainText()+ QString::number(x) +
            " не равно значению текущего узла: " + QString::number(RootBT(b))+"\n");

        if (x>RootBT(b))
        {
             ui->textEdit_2->setText(ui->textEdit_2->toPlainText() +
                "Искомый элемент больше, идем НАПРАВО"
                "\n------------------------------------------------------------------------\n");
             if (Right(b) != NULL)
                bsearchBinTree(Right(b),x, path+"Направо\n");
        }
        else
        {
            if (x<RootBT(b))
            {
                ui->textEdit_2->setText(ui->textEdit_2->toPlainText() +
                    "Искомый элемент меньше, идем НАЛЕВО"
                    "\n------------------------------------------------------------------------\n");
                if (Left(b) != NULL)
                    bsearchBinTree(Left(b),x, path+"Налево\n");
            }

        }

    }
    else
    {
        ui->textEdit_2->setText(ui->textEdit_2->toPlainText()+
            "Элемент " + QString::number(x) + " равен корню текущего поддерева: "
            + QString::number(RootBT(b))+"\n");
        ui->textEdit_2->setText(ui->textEdit_2->toPlainText() + "Найдено совпадение!"
            "\n------------------------------------------------------------------------\n"
            + "Необходимо пройти путь:\n");
        ui->textEdit_2->setText(ui->textEdit_2->toPlainText() + path);
    }
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    ui->textEdit_2->clear();
    bsearchBinTree(b, ui->comboBox->currentText().toInt(), "");
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->textEdit->clear();
    int len = rand()%10 + 12;
    for (int i=0; i < len; i++)
    {
        ui->textEdit->setText(ui->textEdit->toPlainText() + QString::number(rand()%1000) + " ");
    }
}


void MainWindow::on_action_2_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"), QString(),
               tr("Text Files (*.txt)"));

       if (!fileName.isEmpty())
       {
           QFile file(fileName);
           if (!file.open(QIODevice::WriteOnly))
           {
               QMessageBox::critical(this, tr("Ошибка"), tr("Не могу открыть данный файл"));
               return;
           } else
           {
               QTextStream stream(&file);
               stream << ui->textEdit_2->toPlainText();
               stream.flush();
               file.close();
           }
       }
}

void MainWindow::on_action_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"), QString(),
                tr("Text Files (*.txt)"));

        if (!fileName.isEmpty())
        {
            QFile file(fileName);
            if (!file.open(QIODevice::ReadOnly))
            {
                QMessageBox::critical(this, tr("Ошибка"), tr("Не могу открыть данный файл"));
                return;
            }
            QTextStream in(&file);
            ui->textEdit->setText(in.readAll());
            file.close();
        }
}

void MainWindow::on_action_3_triggered()
{
    QMessageBox::about(this, tr("О программе"), tr("Необходимо задать случайное бинарное дерево поиска путем вставки и исключения случайных чисел, генерируемых в программе или вводимых пользователем и продемонстрировать поиск в полученном бинарном дереве заданного числа."));
    return;
}

void MainWindow::on_action_4_triggered()
{
    QMessageBox::about(this, tr("Об авторе"), tr("Курсовая работа\n\nВыполнил: Ковынев М.В.\nГруппа: 6304"));
    return;
}
