#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphwidget.h"
#include <QColorDialog>


double PI = 3.14159265358979323846264338327950288419717;
int flag = 0;
QColor vv(255, 223, 0);
QColor vv2(0,0,0);
GraphWidget *widget;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_3->setDisabled(1);
    ui->pushButton_5->setDisabled(1);
    ui->textEdit_2->setPlaceholderText("Введите матрицу здесь");
}

MainWindow::~MainWindow()
{
    delete ui;
}


// Преобразование текстовой матрицы в вектор
vector<vector<MyNode>> MainWindow::textEditToVector()
{

    // Считывание отображения имен
    QStringList namesList = ui->textEdit_3->toPlainText()
            .replace(":", "\n").replace("\n", " ").split(" ");


    // Создание списка имен
    vector<QString> vector_names;
    for (int i = 2; i< namesList.size(); i = i+3)
    {
        vector_names.push_back(namesList[i]);
    }


    // Считывание матрицы, расчет размеров
    QString str = ui->textEdit_2->toPlainText().replace("\n", " ");
    unsigned int size = 0;
    for (int i=0; i< str.size(); i++)
        if (str[i] == '[')
            size++;


     QStringList list = str.split(' ');
     vector<vector<MyNode>> matrix(1, vector<MyNode>(0));


    // Проверка на совпадение размеров матрицы и списка имен
    if (size > vector_names.size())
    {
        QMessageBox::critical(this, tr("Ошибка"), tr("Несовпадение размеров"
                   " матрицы и списка имен!\nПроверьте корректность "
                   "заданной матрицы и списка соответствия имен."));
        return matrix;
    }


    if (size == 0)
    {   
        return matrix;
    }



    int j = 0;
    int k = 0 ;

    matrix.resize(size);
    for (unsigned int i=0; i<  size; i++)
        matrix[i].resize(size);

    double angle = 2 * PI / matrix[0].size();

    for (int i=0; i < list.size(); i++)
    {
        if (list[i] != "[" && list[i] != "")
        {
            if (list[i] != "]")
            {
                QString x = list[i];

                // Создание нового компонента
                matrix[j][k].value = x.toInt();
                matrix[j][k].name = vector_names[k];
                matrix[j][k].pos.setX(300 + 200*cos(angle * k));
                matrix[j][k].pos.setY(300 + 200*sin(angle * k));
                matrix[j][k].vv = vv;
                matrix[j][k].vv2 = vv2;
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

// Отрисовка графа
void MainWindow::on_pushButton_clicked()
{
    ui->comboBox->clear();
    ui->textEdit_4->clear();

    // Преобразование введеной матрицы в необходимую структуру
    vector<vector<MyNode>> matrix = textEditToVector();
    if (matrix[0].size() != 0)
    {
        // Установка widget
        widget = new GraphWidget(matrix, ui->textEdit, flag, this);
        ui->scrollArea->setWidget(widget);
    }
    else
    {
        ui->textEdit_2->setPlaceholderText("Матрица некорректна!\nВведите другие данные!");
    }
}


// Считывание выбранных вершин и соединение их
void MainWindow::on_textEdit_textChanged()
{
    QString str = ui->textEdit->toPlainText();
    QStringList list = str.split(" ");
    int count = list.size() - 1;     // Количество выбранных вершин


    // Установка кнопок в зависимости от кол-ва выбранных вершин
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

// Генерация матрицы
void MainWindow::on_pushButton_2_clicked()
{
    ui->comboBox->clear();
    ui->textEdit_2->clear();
    ui->textEdit->clear();
    ui->pushButton_3->setDisabled(1);
    ui->pushButton_5->setDisabled(1);

    QString x = "";
    QString x1 = "";
    int size = rand()%6 + 2;


    if (flag == 1)
    {
        // Генерация матрицы из 0 и 1 в случае ориентированного графа
        for (int i=0; i< size; i++)
        {
            x += "[ ";
            for (int j=0; j< size; j++)
            {
                if (!(rand()%3))
                x = x  + QString::number(1) + " ";
                else
                x = x  + QString::number(0) + " ";
            }
            x += "]\n";

        }
        ui->textEdit_2->setText(x);

    }
    else
    {

        // Создание симметричной матрицы для неориетированного
        vector<vector<QString>> a(size, vector<QString>(size, "0"));

        // Создание нижнего левого угла
        for (int i=0; i < size; i++)
        {
            for (int j=i ; j<size; j++)
            {
                a[i][j] = QString::number(rand()%2);
            }
        }

        // Создание верхнего правого угла
        for (int i=0; i < size; i++)
        {
            for (int j=0 ; j<=i; j++)
            {
                a[i][j] = a[j][i];
            }
        }


        // Вывод
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

// Обновление матрицы в окне textEdit на основе переданного вектора
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


// Добавление или удаление в матрице ребра
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

// Соединить 2 вершины по клику
void MainWindow::on_pushButton_3_clicked()
{
   vector<vector<MyNode>> a = textEditToVector();
   QString str = ui->textEdit->toPlainText();

   changeMatrix(a, str, 1);
   widget = new GraphWidget(a, ui->textEdit, flag, this);
    ui->scrollArea->setWidget(widget);

  // ui->graphicsView->setScene(widget->scene());
   ui->textEdit->clear();
   updateMatrix(a);
}


// Соединение всех вершин
void MainWindow::on_pushButton_4_clicked()
{
    QPointF p(0.0,0.0);
    vector<vector<MyNode>> a = textEditToVector();
    vector<vector<MyNode>> b(a[0].size(), vector<MyNode>(a[0].size(), {"", 1, p}));
    updateMatrix(b);
    on_pushButton_clicked();
}


// Удалить выбранное ребро
void MainWindow::on_pushButton_5_clicked()
{
    vector<vector<MyNode>> a = textEditToVector();
    QString str = ui->textEdit->toPlainText();
    changeMatrix(a, str, 0);

     widget = new GraphWidget(a, ui->textEdit, flag, this);
     ui->scrollArea->setWidget(widget);

    ui->textEdit->clear();
    updateMatrix(a);
}

// Удалить все ребра
void MainWindow::on_pushButton_6_clicked()
{
    vector<vector<MyNode>> a = textEditToVector();
    vector<vector<MyNode>> b(a[0].size(), vector<MyNode>(a[0].size()));
    updateMatrix(b);
    on_pushButton_clicked();
}



// Чтение из файла
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


// Сохранение в файл
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


// Об авторе
void MainWindow::on_action_4_triggered()
{
    QMessageBox::about(this, tr("Об авторе"), tr("Лабораторная работа №2\n\nВыполнил: Ковынев М.В.\nГруппа: 6304"));
        return;
}

// О программе
void MainWindow::on_action_3_triggered()
{
    HelpBrowser brows(":/help/help","help.htm");
    brows.exec();
}


// Нахождение компонент двусвязности
void MainWindow::on_pushButton_7_clicked()
{

    // Струкура хранения данных о ребре
    // x - вершина от куда
    // y - вершина куда
     struct XY
     {
         int x;
         int y;
     };


     // Считывание матрицы
      ui->comboBox->clear();
      ui->textEdit_4->clear();
      vector<vector<MyNode>> a = textEditToVector();


      int size = 0;
      if (a[0].size() != 0)
      {
          for (unsigned int i=0; i < a[0].size(); i++)
          {
              for (unsigned int j=0; j < i; j++)
                  if (a[i][j].value == 1)
                      size++;
          }

          // Создание вектора с необходимой структурой для хранения
          // Рассматривается нижняя левая часть матрицы в виду симметрии
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


          // Переберем все подмножества подграфов и проверим их на двусвязность
          for (unsigned int i=0; i<k; i++)
          {

              // Создадим отображение пользовательских имен вершин к реальных именам
              // используемых в программе
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


                    // Выделяем подграф и выполняем проверку на двусвязность
                    Graph g1(tt.size());

                    for (int j=0; j<size; j++)
                    {
                        if (i & (1<<j))
                        {
                            g1.addEdge(tt[b[j].x] , tt[b[j].y]);
                        }
                    }

                    // Если подграф - двусвязен, то выводим сообщение об этом
                    // Добавляем новый элемент в ComboBox
                    if ( g1.isBC())
                    {
                       QString yy;
                       x = x+  "\nНайдена новая компонента двусвязности:\nКоличество вершин: " + QString::number(counter + 1)+ "\nСписок ребер:\n{ ";
                       for (int j=0; j<size; j++)
                       {
                           if (i & (1<<j))
                           {
                               yy = yy + "(" + a[b[j].x][b[j].x].name + ", " +a[b[j].y][b[j].y].name + ") ";
                               x = x + "(" + a[b[j].x][b[j].x].name + ", " +a[b[j].y][b[j].y].name + ") ";
                           }
                       }

                       ui->comboBox->addItem(yy);
                       x+="}\n_________________________________________________________\n";
                    }
                }
          }
          ui->textEdit_4->setText(x);
    }
}

// Переключение выбора неориентированного и
// ориентированного графа
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


// Присваивание нового цвета
void MainWindow::on_pushButton_8_clicked()
{

    vv = QColorDialog::getColor();
    on_pushButton_clicked();
}


// Выделение подграфа
void MainWindow::on_pushButton_9_clicked()
{

     // Считывание матрицы
        vector<vector<MyNode>> matrix = textEditToVector();
        for (int i=0; i< matrix[0].size(); i++)
            for (int j=0; j< matrix[0].size(); j++)
               { matrix[i][j].vv= vv;
                matrix[i][j].vv2= vv2;
            }


       // Считывание отображения имен и строки со списком ребер
        QStringList edge = ui->comboBox->currentText()
                .replace("(", "\n").replace(",", "\n").replace(":", "\n").replace(")", "\n").replace("\n", " ").split(" ");

        QStringList namesList = ui->textEdit_3->toPlainText()
                .replace(":", "\n").replace("\n", " ").split(" ");

        QMap<QString, int> mapper;
        for (int i = 2; i< namesList.size(); i = i+3)
        {
            mapper[namesList[i]] = (i-2)/3+1;
        }


        // Заполняем цветом
        QColor tt(0, 223, 0);
        for (int i=1; i<edge.size() - 1; i = i +5)
        {
            matrix[mapper[edge[i]] - 1][mapper[edge[i]] - 1].vv = tt;
            matrix[mapper[edge[i+2]] - 1][mapper[edge[i+2]] - 1].vv = tt;
            matrix[mapper[edge[i]] - 1][mapper[edge[i+2]] - 1].vv2 = tt;

        }


        // Создание widgetа
        if (matrix[0].size() != 0)
        {
            widget = new GraphWidget(matrix, ui->textEdit, flag,  this);
            ui->scrollArea->setWidget(widget);
        }

}
