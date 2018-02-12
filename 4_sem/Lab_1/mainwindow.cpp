#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "solver.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}


int colorist = 1;
int counter  = 1;
int maxindex = 1;
int minindex = 0;

void MainWindow::on_pushButton_clicked()
{
     remove("output_0.ppm");
     remove("output_1.ppm");
     remove("output_2.ppm");
     remove("output_3.ppm");
     remove("output_4.ppm");
     remove("output_5.ppm");
     remove("output_6.ppm");


     int value = ui->textEdit->toPlainText().toInt();

     ui->warning->setText("Происходит генерация решения.\nПожалуйста, подождите.");
     if (value <= 42 && value >=1 )
     {

         ui->progressBar->setValue(0);
         maxindex = mainsolver(ui->textEdit->toPlainText().toInt(), colorist, ui->progressBar, ui->sol);

         counter = 1;
         QPixmap image("output_0.ppm");
         ui->label->clear();
         ui->label->setPixmap(image);

         QPixmap image1("output_1.ppm");
         ui->step->setText("Шаг 1");
         ui->Current->clear();
         ui->Current->setPixmap(image1);

         window()->setFixedSize(1150,390);
         ui->warning->setText("Генерация завершена.\nУспех!");
     }
     else
     {
          ui->label->clear();
          ui->Current->clear();
          ui->progressBar->setValue(0);
          ui->warning->setText("Невозможно решить задачу.\nВведите меньшее число");
          ui->step->setText("Шаг 1");
          window()->setFixedSize(190,390);

     }
}


void MainWindow::on_pushButton_next_clicked()
{
    if (maxindex > counter +1 )
    {
        counter++;
        char fileNameIndex[3];
        sprintf(fileNameIndex, "%d", counter);
        char fileName[30] = "output_";   // Создание имени изображения
        strcat(fileName, fileNameIndex);  // с учетом его номера
        strcat(fileName, ".ppm");

        QPixmap image(fileName);
        ui->Current->setPixmap(image);
        ui->step->setText("Шаг "+QString::number(counter));
    }
}

void MainWindow::on_pushButton_prev_clicked()
{
    if (minindex < counter -1)
    {
        counter--;
        char fileNameIndex[3];
        sprintf(fileNameIndex, "%d", counter);
        char fileName[30] = "output_";   // Создание имени изображения
        strcat(fileName, fileNameIndex);  // с учетом его номера
        strcat(fileName, ".ppm");

        QPixmap image(fileName);
        ui->Current->setPixmap(image);
        ui->step->setText("Шаг "+QString::number(counter));
    }
}

void MainWindow::on_radioButton_clicked()
{
    if (colorist == 1)
        colorist = 0;
    else
        colorist = 1;
}

void MainWindow::on_pushButton_4_clicked()
{
    exit(0);
}

void MainWindow::on_pushButton_2_clicked()
{
    QMessageBox::about(this, tr("Справка"), tr("Задача\n\nУ Томми есть много бумажных квадратиков. Длина из стороны (размер) изменяется от 1 до N – 1, и у него есть неограниченное число квадратов любого размера. Но ему очень хочется получить большой квадрат – размера N.\n\nОн может получить такой квадрат, построив его из уже имеющихся квадратов. Например, квадрат размера 7 может быть построен из 9 меньших квадратов. Внутри квадрата не должно быть пустого места, меньшие квадраты не должны выходить за пределы большего и не должны перекрываться. Кроме того Томми хочет истратить минимально возможное число квадратов."));
         return;
}

void MainWindow::on_pushButton_3_clicked()
{
    QMessageBox::about(this, tr("О программе"), tr("Выполнил:  Ковынев М.В.\nГруппа: 6304"));
         return;
}
