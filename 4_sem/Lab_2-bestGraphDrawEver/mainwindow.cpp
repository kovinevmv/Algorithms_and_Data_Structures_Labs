#include "mainwindow.h"
#include "biconnected.h"
#include "ui_mainwindow.h"
#include "graphwidget.h"

double PI = 3.14159265358979323846264338327950288419717;
int flag = 0;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_3->setDisabled(1);
    ui->pushButton_5->setDisabled(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Text to Vector
vector<vector<MyNode>> MainWindow::textEditToVector()
{
    QStringList namesList = ui->textEdit_3->toPlainText()
            .replace(":", "\n").replace("\n", " ").split(" ");

    vector<QString> vector_names;
    for (int i = 2; i< namesList.size(); i = i+3)
    {
        vector_names.push_back(namesList[i]);
    }

    QString str = ui->textEdit_2->toPlainText().replace("\n", " ");
    int size = count(begin(str), end(str), "[");

    vector<vector<MyNode>> matrix(1, vector<MyNode>(0));
    if (size == 0)
    {   
        return matrix;
    }

    QStringList list = str.split(' ');

    int j = 0;
    int k = 0 ;

    matrix.resize(size);
    for (int i=0; i<  size; i++)
        matrix[i].resize(size);

    double angle = 2 * PI / matrix[0].size();

    for (int i=0; i < list.size(); i++)
    {
        if (list[i] != "[" && list[i] != "")
        {
            if (list[i] != "]")
            {
                QString x = list[i];
                matrix[j][k].value = x.toInt();
                matrix[j][k].name = vector_names[k];
                matrix[j][k].pos.setX(300 + 200*cos(angle * k));
                matrix[j][k].pos.setY(300 + 200*sin(angle * k));
                k++;
            }
            else
            {
                j++;
                k=0;
            }
        }
    }

    return matrix;
}

// Draw graph
void MainWindow::on_pushButton_clicked()
{
    ui->textEdit_4->clear();
    vector<vector<MyNode>> matrix = textEditToVector();
    if (matrix[0].size() != 0)
    {
        GraphWidget *widget = new GraphWidget(matrix, ui->textEdit, flag, this);
        ui->graphicsView->setScene(widget->scene());
    }
    else
    {
        ui->textEdit_2->setPlaceholderText("Матрица некорректна!\nВведите другие данные!");
    }
}


// Get elements with double click
void MainWindow::on_textEdit_textChanged()
{
    QString str = ui->textEdit->toPlainText();
    QStringList list = str.split(" ");
    int count = list.size() - 1;

    if (count > 2)
    {
        ui->textEdit->clear();
        ui->pushButton_3->setDisabled(1);
        ui->pushButton_5->setDisabled(1);
    }
    else
    if (count == 2)
    {
        ui->pushButton_3->setEnabled(1);
        ui->pushButton_5->setEnabled(1);
    }
    else
    if (count < 2)
    {
         ui->pushButton_3->setDisabled(1);
         ui->pushButton_5->setDisabled(1);
    }

}

// Generate TextEdit
void MainWindow::on_pushButton_2_clicked()
{
    ui->textEdit_2->clear();
    ui->textEdit->clear();
    ui->pushButton_3->setDisabled(1);
    ui->pushButton_5->setDisabled(1);

    QString x = "";
    QString x1 = "";
    int size = rand()%10 + 2;

    if (flag == 1)
    {

        for (int i=0; i< size; i++)
        {
            x += "[ ";
            for (int j=0; j< size; j++)
            {
                x = x  +QString::number(rand()%2) + " ";
            }
            x += "]\n";

        }
        ui->textEdit_2->setText(x);

    }
    else
    {
        vector<vector<QString>> a(size, vector<QString>(size, "0"));
        for (int i=0; i < size; i++)
        {
            for (int j=i ; j<size; j++)
            {
                a[i][j] = QString::number(rand()%2);
            }
        }
        for (int i=0; i < size; i++)
        {
            for (int j=0 ; j<=i; j++)
            {
                a[i][j] = a[j][i];
            }
        }

        for (int i=0; i < size; i++)
        {
            x += "[ ";
            for (int j=0; j< size; j++)
            {
                x = x  + a[i][j] + " ";
            }
            x += "]\n";
        }
         ui->textEdit_2->setText(x);

    }
    for (int i=0; i< size; i++)
    {
        x1 = x1 + QString::number(i+1) + ": " + QString::number(i+1) + "\n";
    }
     ui->textEdit_3->setText(x1);
}


void MainWindow::updateMatrix(const vector<vector<MyNode> > &a)
{
    ui->textEdit_2->clear();
    QString x = "";
    int size = a[0].size();
    for (int i=0; i< size; i++)
    {
        x += "[ ";
        for (int j=0; j< size; j++)
        {
            x = x + QString::number(a[i][j].value) + " ";
        }
       x +="]\n";
    }

     ui->textEdit_2->setText(x);
}


void changeMatrix(vector<vector<MyNode>>& a, const QString& str, int change)
{
    QStringList list = str.split(' ');

    int ii, jj;
    for (unsigned int i=0; i< a[0].size(); i++)
    {
        if (a[0][i].name == list[0])
            ii = i;
        if (a[0][i].name == list[1])
           jj = i;

    }

     a[ii][jj].value =  change;
    if (flag == 0)
    {
         a[jj][ii].value =  change;
    }

}

// Connect 2 Nodes
void MainWindow::on_pushButton_3_clicked()
{
   vector<vector<MyNode>> a = textEditToVector();
   QString str = ui->textEdit->toPlainText();
   changeMatrix(a, str, 1);
   GraphWidget *widget = new GraphWidget(a, ui->textEdit, flag, this);
   ui->graphicsView->setScene(widget->scene());
   ui->textEdit->clear();
   updateMatrix(a);
}


// Connect All Nodes
void MainWindow::on_pushButton_4_clicked()
{
    QPointF p(0.0,0.0);
    vector<vector<MyNode>> a = textEditToVector();
    vector<vector<MyNode>> b(a[0].size(), vector<MyNode>(a[0].size(), {"", 1, p}));
    updateMatrix(b);
    on_pushButton_clicked();
}


// Remove 1 Edge
void MainWindow::on_pushButton_5_clicked()
{
    vector<vector<MyNode>> a = textEditToVector();
    QString str = ui->textEdit->toPlainText();
    changeMatrix(a, str, 0);
    GraphWidget *widget = new GraphWidget(a, ui->textEdit, flag, this);
    ui->graphicsView->setScene(widget->scene());
    ui->textEdit->clear();
    updateMatrix(a);
}

// Remove All Edges
void MainWindow::on_pushButton_6_clicked()
{
    vector<vector<MyNode>> a = textEditToVector();
    vector<vector<MyNode>> b(a[0].size(), vector<MyNode>(a[0].size()));
    updateMatrix(b);
    on_pushButton_clicked();
}


void MainWindow::on_action_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"), QString(),
                    tr("Text Files (*.txt)"));

    ui->textEdit_2->clear();
    ui->textEdit_3->clear();

    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(this, tr("Ошибка"), tr("Не могу открыть данный файл"));
            return;
        }
        QTextStream in(&file);
        QString x;
        while (1)
        {
            x = in.readLine();
            if (x != "#############")
                ui->textEdit_2->setText(ui->textEdit_2->toPlainText() + x + "\n");
            else
                break;
        }
        while (1)
        {
            x = in.readLine();
            if (x != "#############")
                ui->textEdit_3->setText(ui->textEdit_3->toPlainText() + x + "\n");
            else
                break;
        }
       file.close();
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

                  stream << "\n#############\n";
                  stream << ui->textEdit_3->toPlainText();
                  stream << "\n#############\n";
                  stream.flush();
                  file.close();
              }
          }
}

void MainWindow::on_action_4_triggered()
{
    QMessageBox::about(this, tr("Об авторе"), tr("Лабораторная работа №2\n\nВыполнил: Ковынев М.В.\nГруппа: 6304"));
        return;
}

void MainWindow::on_action_3_triggered()
{
    QMessageBox::about(this, tr("О программе"),
                 tr("Данная программа рисует ориентированный граф. "
                 "\n\Возможности:\n1. Сохранение графа в файл\n2."
                 " Возможность создание и удаления ребер по двойному "
                 "клику вершин\n3. Замена имени вершины графа на собственную\n4. "
                 "Поиск компонент двусвязности"));
        return;
}

void MainWindow::on_pushButton_7_clicked()
{
     struct XY
     {
         int x;
         int y;
     };

      ui->textEdit_4->clear();
      vector<vector<MyNode>> a = textEditToVector();
      int size = 0;
      if (a[0].size() != 0)
      {
          for (unsigned int i=0; i < a[0].size(); i++)
          {
              for (int j=0; j < i; j++)
                  if (a[i][j].value == 1)
                      size++;
          }
          vector<XY> b(size);
          unsigned long long int k= 0;
          for (unsigned int i=0; i < a[0].size(); i++)
          {
              for (unsigned int j=0; j < i; j++)
                  if (a[i][j].value == 1)
                  {
                      b[k].x = i;
                      b[k].y = j;
                      k++;
                  }
          }


          QString x= "";

          int counter = 0;
          k=powf(2,size);

          for (unsigned int i=0; i<k; i++)
          {


                map <int, int> tt;
                counter = 0;
                for (int j=0; j<size; j++)
                {
                    if (i & (1<<j))
                    {
                        counter++;
                        tt[b[j].y] = 1;
                        tt[b[j].x] = 1;
                    }
                }


                if (counter != 0 )
                {
                    int y =0;
                    for (auto& t: tt)
                    {
                        t.second = y;
                        y++;
                    }

                    Graph g1(tt.size());
                    x = x+  "Количество вершин: " + QString::number(counter + 1)+ "\nСписок ребер:\n{";
                    for (int j=0; j<size; j++)
                    {
                        if (i & (1<<j))
                        {
                            x = x + "(" + a[b[j].x][b[j].x].name + ", " +a[b[j].y][b[j].y].name + ") ";
                            g1.addEdge(tt[b[j].x] , tt[b[j].y]);

                        }
                    }

                    if ( g1.isBC())
                    {
                       x  +=  "}\nПодграф - компонента двусвязности\n";
                    }
                    else
                    {
                      x  +=  "}\nПодграф - НЕ компонента двусвязности\n";
                    }
                    x+="######################################\n\n";
                }
          }
          ui->textEdit_4->setText(x);
    }
}

void MainWindow::on_radioButton_clicked()
{
    if (flag == 1)
    {
        flag = 0;
        ui->pushButton_7->setDisabled(0);
    }
    else
    {
         ui->pushButton_7->setDisabled(1);
          flag = 1;
    }
}
