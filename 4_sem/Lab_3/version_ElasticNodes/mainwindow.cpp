#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphwidget.h"
#include <QColorDialog>
#include <sstream>

double PI = 3.14159265358979323846264338327950288419717;
int flag = 0;
QColor vv(255, 223, 0);
QColor vv2(0,0,0);
GraphWidget *widget;
bool processing = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_3->setDisabled(1);
    ui->pushButton_5->setDisabled(1);
    ui->textEdit_2->setPlaceholderText("Введите матрицу здесь");
    QSplitter* spl = new QSplitter(Qt::Horizontal);
    spl->addWidget(ui->textEdit_2);
    spl->addWidget(ui->textEdit_3);
    spl->resize(200, 200);
    ui->scrollArea_2->setWidget(spl);


    rotableGroupBox = new QGroupBox(tr("Rotable Widgets"));

    rotableWidgets.append(ui->scrollArea);
   //rotableWidgets.enqueue(ui->scrollArea);
    rotableWidgets.enqueue(ui->label_5);
    rotableWidgets.enqueue(ui->comboBox);
    rotableWidgets.enqueue(ui->comboBox_2);
    rotableLayout = new QGridLayout;
    rotableGroupBox->setLayout(rotableLayout);



    foreach (QWidget *widget, rotableWidgets)
        rotableLayout->removeWidget(widget);

    rotableWidgets.enqueue(rotableWidgets.dequeue());

    const int n = rotableWidgets.count();
    qDebug() << n;
    for (int i = 0; i < n / 2; ++i) {
        rotableLayout->addWidget(rotableWidgets[n - i - 1], 0, i);
        rotableLayout->addWidget(rotableWidgets[i], 1, i);
    }

    mainLayout = new QGridLayout;
    mainLayout->addWidget(rotableGroupBox, 0, 0);
    setLayout(mainLayout);



}

MainWindow::~MainWindow()
{
    delete ui;
}


bool checkCorrectInputTxt(string input)
{
    stringstream ss(input);
    string firststring;
    int count = 0;
    processing = 1;
    while(getline(ss, firststring) && processing)
    {
        count++;
        if (firststring[0] != '[')
        {
              int ret = QMessageBox::warning(0, "Ошибка открытия файла!",
              "Отсутствует \"[\" в начале " + QString::number(count) + " строки.\nДля создания графа необходимо создать корректную матрицу. "
              "\nВид матрицы описан в Справке.\nОткрыть Справку?", QMessageBox::Open | QMessageBox::Cancel);

            switch (ret)
            {
              case QMessageBox::Open:
                  HelpBrowser brows(":/help/help","about_graph.htm");
                  brows.exec();
                  break;
            }
            return 0;
        }
        else
        {

            if (firststring[firststring.size()-1] != ']')
            {
                int ret = QMessageBox::warning(0, "Ошибка открытия файла!",
                  "Отсутствует \"]\" в конце " + QString::number(count) + " строки.\nДля создания графа необходимо создать корректную матрицу. "
                  "\nВид матрицы описан в Справке.\nОткрыть Справку?", QMessageBox::Open | QMessageBox::Cancel);

                switch (ret)
                {
                  case QMessageBox::Open:
                      HelpBrowser brows(":/help/help","about_graph.htm");
                      brows.exec();

                      break;
                }
                 return 0;
            }
        }
    }
    processing = 0;
    return 1;
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
    ui->comboBox_2->clear();
    ui->plainTextEdit->clear();
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
    MainWindow::on_pushButton_7_clicked();
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
    ui->comboBox_2->clear();
    ui->plainTextEdit->clear();
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
                x = x  + QString::number(rand()%9 + 1) + " ";
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
                if (!(rand()%3))
                    a[i][j] = QString::number(rand()%9 + 1);
                else
                    a[i][j] =  QString::number(0);
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
   MainWindow::on_pushButton_7_clicked();
}

// Соединение всех вершин
void MainWindow::on_pushButton_4_clicked()
{
    QPointF p(0.0,0.0);
    vector<vector<MyNode>> a = textEditToVector();
    vector<vector<MyNode>> b(a[0].size(), vector<MyNode>(a[0].size(), {"", 1, p, {0,0,0}, {0,0,0}}));
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
    MainWindow::on_pushButton_7_clicked();
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
    ui->textEdit_2->clear();
    ui->textEdit_3->clear();
    QString FileName = QFileDialog::getOpenFileName(this, "Открыть файл...",QString(), "Text files (*.txt)");
    if (FileName.isEmpty())
        return;

    QFile File(FileName);

    if(File.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString x = File.readAll();
        if (checkCorrectInputTxt(x.toLocal8Bit().constData()))
        {
            ui->textEdit_2->setPlainText(x);
            File.close();
        }

    }

    QString parseName =  "";
    for (int i=1; i< 25; i++)
    {
        parseName += (QString::number(i) + ": " + QString::number(i) + "\n");
    }
    ui->textEdit_3->setPlainText(parseName);
}

quint32 key = 1488; //шифр
QString encodeStr(const QString& str)
{
    QByteArray arr(str.toUtf8());
    for(int i =0; i<arr.size(); i++)
        arr[i] = arr[i] ^ key;
    return QString::fromUtf8(arr.toBase64());
}

QString decodeStr(const QString &str)
{
    QByteArray arr = QByteArray::fromBase64(str.toLocal8Bit());
       for(int i =0; i<arr.size(); i++)
        arr[i] =arr[i] ^ key;
    return QString::fromUtf8(arr);

}

// Сохранение в файл форматированного графа
void MainWindow::on_action_2_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"), QString(),
                  tr("My Graf (*.graf)"));

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

                  stream << encodeStr(ui->textEdit_2->toPlainText()) + "\n";
                  stream << encodeStr("#############");
                  stream << "\n";
                  stream << encodeStr(ui->textEdit_3->toPlainText());
                  stream << "\n";
                  stream << encodeStr("#############");
                  stream << "\n";
                  stream << encodeStr(QString::number(vv.red()) + " " +  QString::number(vv.green()) + " "  + QString::number(vv.blue()));
                  stream << "\n";

                  stream.flush();
                  file.close();
              }
          }
}

// Об авторе
void MainWindow::on_action_4_triggered()
{


        QDialog* inf  = new QDialog(this);

        inf->setWindowModality(Qt::ApplicationModal);
        QHBoxLayout * lay = new QHBoxLayout;
        QLabel* e = new QLabel;
        e->setFixedSize(180,180);

        QMovie * mov = new QMovie(":help/help/me.gif");
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

// О программе
void MainWindow::on_action_3_triggered()
{
    HelpBrowser brows(":/help/help","1.htm");
    brows.exec();
}

// Нахождение
void MainWindow::on_pushButton_7_clicked()
{
    ui->comboBox->clear();
    ui->comboBox_2->clear();
    ui->plainTextEdit->clear();

    vector<vector<MyNode>> a = textEditToVector();

    QString l=ui->textEdit_2->toPlainText();
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

        QString finded = pathfinder(F,  N, next, d);
        ui->textEdit_4->setPlainText(finded);



    }
    else
        ui->textEdit_4->setPlainText("error");

    for (unsigned int i=0; i<a[0].size(); i++)
    {
        ui->comboBox->addItem(a[i][i].name);
        ui->comboBox_2->addItem(a[i][i].name);
    }


}

// Переключение выбора неориентированного и
// ориентированного графа
void MainWindow::on_radioButton_clicked()
{
    if (flag == 1)
    {
        flag = 0;

    }
    else
    {

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
        vector<vector<MyNode>> a = textEditToVector();
        for (unsigned int i=0; i< a[0].size(); i++)
            for (unsigned int j=0; j< a[0].size(); j++)
            {
                a[i][j].vv= vv;
                a[i][j].vv2= vv2;
            }


        int qq=1;
        int NN = a[0].size();
        int* array = new int [NN];
        QString l1,l2;

        int N=0;
        QString l=ui->textEdit_2->toPlainText();
        if (l.size()>10)
        {
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
            ui->textEdit_4->setPlainText(finded);


        QString l1 = ui->comboBox->currentText();
        QString l2 = ui->comboBox_2->currentText();
        if ((l1.size()>0)&&(l2.size()>0))
        {

            QStringList namesList = ui->textEdit_3->toPlainText()
                            .replace(":", "\n").replace("\n", " ").split(" ");

            QMap<QString, int> mapper;
            QMap<int, QString> reversedMapper;
            for (int i = 2; i< namesList.size(); i = i+3)
            {
                 mapper[namesList[i]] = (i-2)/3+1;
                 reversedMapper[(i-2)/3+1] = namesList[i];
            }

            int u = mapper[l1]-1;
            int v = mapper[l2]-1;
            int c;
            if (u>=N or v>=N)
            {
                ui->plainTextEdit->setPlainText("Не найдены вершины! Повторите попытку.");
                widget = new GraphWidget(a, ui->textEdit, flag,  this);
                ui->scrollArea->setWidget(widget);
                return;
            }
            if (d[u][v] == 0)
            {
                ui->plainTextEdit->setPlainText("Путь между заданными вершинами не существует!");
                widget = new GraphWidget(a, ui->textEdit, flag,  this);
                ui->scrollArea->setWidget(widget);
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
                ui->plainTextEdit->setPlainText(path);
            }

                // Считывание отображения имен и строки со списком ребер
                    QStringList edge = ui->plainTextEdit->toPlainText().replace("-", "\n")
                            .replace(">", "\n").replace("=", "\n").remove(QRegExp("^\s*")).replace("\n", " ").split(" ");

                   vector<pair<QString, QString>> arrayOfEdges;
                    for (int i = 0; i < edge.size() - 2; i = i +3)
                    {
                        arrayOfEdges.push_back({edge[i],edge[i+3]});
                    }




                    // Заполняем цветом
                    QColor tt(0, 255, 0);
                    for (unsigned int i=0; i<arrayOfEdges.size(); i++)
                    {
                        a[arrayOfEdges[i].first.toInt()-1][arrayOfEdges[i].first.toInt()-1].vv = tt;
                        a[arrayOfEdges[i].second.toInt()-1][arrayOfEdges[i].second.toInt()-1].vv = tt;
                        a[arrayOfEdges[i].first.toInt()-1][arrayOfEdges[i].second.toInt()-1].vv2 = tt;

                    }


                    // Создание widgetа
                    if (a[0].size() != 0)
                    {
                        widget = new GraphWidget(a, ui->textEdit, flag,  this);
                        ui->scrollArea->setWidget(widget);
                    }

                    QString printPath = "Порядок посещения вершин:\n";
                    ui->plainTextEdit->clear();
                    for (int i=0; i< edge.size()-3; i=i+3)
                    {
                        printPath += (reversedMapper[edge[i].toInt()] + " -> ");
                    }
                    printPath += (" " + reversedMapper[edge[edge.size()-2].toInt()] + "\nРасстояние: " +
                            QString::number(edge[edge.size()-1].toInt()));
                    ui->plainTextEdit->setPlainText(printPath);



        }




    }

}



void MainWindow::on_action_5_triggered()
{
    ui->textEdit_2->clear();
    ui->textEdit_3->clear();
    QString FileName = QFileDialog::getOpenFileName(this, "Открыть файл...",QString(), "Graf (*.graf)");
    if (FileName.isEmpty())
        return;

    QFile File(FileName);

    if(File.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString x = File.readAll();
            QTextStream stream(&x);
            QString currentEncodedString;
            stream >> currentEncodedString;
            ui->textEdit_2->setPlainText(decodeStr(currentEncodedString));

             stream >> currentEncodedString;
              stream >> currentEncodedString;
              ui->textEdit_3->setPlainText(decodeStr(currentEncodedString));
              stream >> currentEncodedString;
              stream >> currentEncodedString;

              QStringList color = decodeStr(currentEncodedString).split(" ");
              vv.setRed(color[0].toInt());
              vv.setGreen(color[1].toInt());
              vv.setBlue(color[2].toInt());
            File.close();
           on_pushButton_clicked();

    }

}
