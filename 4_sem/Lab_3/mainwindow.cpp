#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QPlainTextEdit>
#include "stringtograph.h"



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

int sizeofarray = 0;
string headerDot = "digraph { graph [pad=\"1.2\", nodesep=\"0.6\", ranksep=\"0.6\"]; "
                 "splines=\"false\"; "
                 "node[shape = circle]; "
                 "edge[style=rounded]; "
                 "ratio=\"fill\"; "
                 "margin=0;\n";


void MainWindow::on_createGraph_clicked()
{

    ofstream f("dot.dot");
    f  << headerDot;

    vector<vector<int>> b = readmatrix(ui->plainTextEdit->toPlainText());
    sizeofarray = b[0].size();

    for (int i=0; i< b.size(); i++)
        for (int j=0; j< b.size(); j++)
        {
            if (b[i][j] != 0)
            {
                QString pathQString = QString::number(i+1) + " -> " + QString::number(j+1) + " [label=\"" +  QString::number(b[i][j]) + "\"];\n";
                string pathString = pathQString.toLocal8Bit().constData();
                f << pathString;
            }
        }

    f << "}";
    f.close();
    system("CD /D \"D:\Documents\Qt Projects\build-graphviz_"
           "beta-Desktop_Qt_5_10_0_MinGW_32bit-Debug\" & "
           "dot -Gsize=3,3\! -Gdpi=300 -Tpng dot.dot -o main.png");

    QPixmap image("main.png");
    ui->label->setPixmap(image);
    ui->label->repaint();
    ui->scrollArea->setWidget(ui->label);

    window()->setFixedSize(1050, 660);
}

void MainWindow::on_pushButton_2_clicked()
{
    QString stringMatrix = "";
    int size = rand()% 5 + 3;

    vector<vector<int>> matrix(size, vector<int>(size, 0));
    for (int i=0; i< size; i++)
    {
         stringMatrix += "[ ";
         for (int j=0; j<size; j++)
         {
             int randomize = rand()% 3;
             if (!randomize)
                matrix[i][j] = rand() % 20;
             stringMatrix = stringMatrix +  QString::number(matrix[i][j]) + " ";
         }
         stringMatrix += "]\n";
    }

    ui->plainTextEdit->setPlainText(stringMatrix);
}

void MainWindow::newrender(int **F, int N, int* array)
{
        generateDotFile(F, N, array, headerDot);

        system("CD /D \"D:\Documents\Qt Projects\build-graphviz_beta-Desktop_Qt_5_10_0_MinGW_32bit-Debug\" & dot -Gsize=3,3\! -Gdpi=300 -Tpng dot2.dot -o rendered.png");

        QPixmap image("rendered.png");
        ui->label->setPixmap(image);
        ui->scrollArea->setWidget(ui->label);
}

void MainWindow::on_pushButton_3_clicked()
{
        int qq=1;
        int NN = sizeofarray;
        int* array = new int [NN];
        QString l1,l2;
        l1=ui->plainTextEdit_2->toPlainText();
        l2=ui->plainTextEdit_3->toPlainText();
        QString l=ui->plainTextEdit->toPlainText();
        if (l.size()>10)
        {

            int N=0;
            for (int i=0;i<l.size();++i)
            {
                if (l[i]=='[')
                    ++N;
            }
            int **F = new int* [N];
            for (int i = 0; i < N; ++i)
                F[i] = new int [N];
            int **next = new int* [N];
            for (int i = 0; i < N; ++i)
                next[i] = new int [N];
            int **d = new int* [N];
            for (int i = 0; i < N; ++i)
                d[i] = new int [N];

            convertMatrix(l,F,N);
            QString path = "|";
            QString finded = pathfinder(F,  N, next, d);
            ui->plainTextEdit_4->setPlainText(finded);

            if ((l1.size()>0)&&(l2.size()>0))
            {
                int u,v;
                bool ok1;
                int n=l1.toInt(&ok1,10) -1 ;
                if (ok1)
                    u=n;
                n=l2.toInt(&ok1,10) - 1;
                if (ok1)
                    v=n;
                int c;
                if (u>=N or v>=N)
                {
                    ui->plainTextEdit_5->setPlainText("No nodes found");
                    QPixmap image("main.png");
                    ui->label->setPixmap(image);
                    ui->scrollArea->setWidget(ui->label);
                    return;
                }
                if (d[u][v] == 0)
                {
                    ui->plainTextEdit_5->setPlainText("No path found");
                    QPixmap image("main.png");
                    ui->label->setPixmap(image);
                    ui->scrollArea->setWidget(ui->label);
                    return;
                }

                else
                {
                    c = u;
                    while (c != v)
                    {
                        if (c==u)
                        {
                            path=QString::number(c+1);
                            array[qq] = c;
                            qq++;
                        }
                        else
                        {
                            path+=QString::number(c+1);
                            array[qq] = c;
                            qq++;
                        }

                            path+="-> ";

                        c = next[c][v];
                    }
                    path+=QString::number(v+1);
                     array[qq] = v;
                     qq++;
                    path+='=';
                    path+=QString::number(d[u][v]);
                    ui->plainTextEdit_5->setPlainText(path);
                }
            }

            array[0] = qq;

            newrender(F, N, array);

            for (int i = 0; i < N; ++i)
                            delete []d[i];
            for (int i = 0; i < N; ++i)
                            delete []next[i];
            for (int i = 0; i < N; ++i)
                            delete []F[i];

        }
        else
            ui->plainTextEdit_2->setPlainText("error");
}

void MainWindow::on_openfile_triggered()
{
    QString FileName = QFileDialog::getOpenFileName(this, "Открыть файл...", QDir::homePath(), "Text files (*.txt)");
    if (FileName.isEmpty())
        return;

    QFile File(FileName);

    if(File.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->plainTextEdit->setPlainText(File.readAll());
        File.close();
    }
    else
    {
        QMessageBox::warning(this, "Error", QString("Не смог открыть файл %1").arg(FileName),QMessageBox::Ok);
    }
}

void MainWindow::on_action_triggered()
{
    QMessageBox::about(this, tr("О программе"), tr("Нахождение кратчайшего пути между\nдвумя вершинами."));
        return;
}

void MainWindow::on_action_2_triggered()
{
    QMessageBox::about(this, tr("Об авторе"), tr("Лабораторная работа №3\n\nВыполнил: Ковынев М.В.\nГруппа: 6304"));
        return;
}
