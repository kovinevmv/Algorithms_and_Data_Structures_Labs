#include "mainwindow.h"
#include "helpbrowser.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "solver.h"
#include <QTimer>

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
int max= 0;

void MainWindow::on_pushButton_clicked()
{

     remove("output_0.ppm");
     remove("output_1.ppm");
     remove("output_2.ppm");
     remove("output_3.ppm");
     remove("output_4.ppm");
     remove("output_5.ppm");
     remove("output_6.ppm");

     // Считывание значения стороны
     int value = ui->textEdit->toPlainText().toInt();


     ui->warning->setText("Происходит генерация решения.\nПожалуйста, подождите.");

     // Проверка на корректность
     if (value <= 42 && value >=1 )
     {

         ui->progressBar->setValue(0);
         maxindex = mainsolver(ui->textEdit->toPlainText().toInt(), colorist, ui->progressBar, ui->sol);


         // Установка изображений и переключателей
         counter = 1;
         QPixmap image("output_0.ppm");
         ui->label->clear();
         ui->label->setPixmap(image);

         QPixmap image1("output_1.ppm");
         ui->step->setText("Шаг 1");
         ui->Current->clear();
         ui->Current->setPixmap(image1);

         window()->setFixedSize(1150,500);
         ui->warning->setText("Генерация завершена.\nУспех!");
     }
     else
     {
         // Очистка экрана
          ui->label->clear();
          ui->Current->clear();
          ui->progressBar->setValue(0);
          ui->warning->setText("Невозможно решить задачу.\nВведите другое число");
          ui->step->setText("Шаг 1");
          window()->setFixedSize(207,500);

     }
}


void MainWindow::on_pushButton_next_clicked()
{

    // Проверка на существование картинки
    if (maxindex > counter +1 )
    {

        // Открытие след. картинки
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

// Проверка на существование картинки
    if (minindex < counter -1)
    {
        // Открытие пред. картинки
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

    // Переключатель цвета
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
    HelpBrowser brows(":/help/help","help.htm");
        brows.exec();
}

void MainWindow::on_pushButton_3_clicked()
{
    QMessageBox::about(this, tr("О программе"), tr("Выполнил:  Ковынев М.В.\nГруппа: 6304"));
         return;
}

QTimer *timer;
void MainWindow::on_pushButton_5_clicked()
{
    // Установка таймера на слот
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_pushButton_next_clicked()));
    for (int i=0; i< maxindex; i++)
         timer->start(1700);
}

void MainWindow::on_pushButton_6_clicked()
{
    // Отключение таймера
    disconnect(timer, SIGNAL(timeout()), this, SLOT(on_pushButton_next_clicked()));
}
