#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QString>
#include <algorithm>

#include <QDebug>
#include "time.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);
    ui->progressBar->setHidden(1);
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::on_pushButton_clicked()
{
    exit(1);
}

double fact(int N)
{
    if(N < 0)
        return 0;
    if (N == 0)
        return 1;
    else
        return N * fact(N - 1);
}


int count=0;
int maxfact;

void MainWindow::generatePermutations(int k, int N, QString* array)
{


    if (k == N-1)
    {
        ui->textEdit_2->setText(ui->textEdit_2->toPlainText()+QString::number(++count)+". ");
        ui->progressBar->setValue(count*100.0/maxfact);
            for (int i = 0; i < N; i++)
            {
                 ui->textEdit_2->setText(ui->textEdit_2->toPlainText()+array[i] + " ");
            }
        ui->textEdit_2->setText(ui->textEdit_2->toPlainText()+"\n");

        }
        else
        {
            for (int j = k; j < N; j++)
            {
                std::swap(array[k], array[j]);
                generatePermutations(k+1, N, array);
                std::swap(array[k], array[j]);
            }
        }


}

void MainWindow::on_pushButton_2_clicked()
{
   ui->textEdit_2->clear();
   ui->progressBar->setHidden(0);

   QString inputString = ui->textEdit->toPlainText().simplified();

   QString* array = new QString[12];
   int y=0;
   int size=0;
   for (int i=0; i < inputString.size(); i++)
   {

       if (inputString[i] == ' ')
       {
           QString x="";
           for (int j=y; j < i; j++)
           {
                x +=inputString[j];
           }

           array[size]=x;
           size++;
           y=i+1;
       }
   }

   QString x="";
   for (int j=y; j < inputString.size(); j++)
   {
        x +=inputString[j];
   }
   array[size]=x;
   size++;

   maxfact = fact(size);

    if (size>=7)
    {
        QMessageBox::critical(this, tr("Ошибка"), tr("Возможное переполнение памяти. Введите меньшее количество чисел"));
        return;
    }


    ui->label_3->setText("Пожалуйста, подождите.\nВыполняется генерация перестановок.");
    ui->label_3->setStyleSheet("QLabel {color : black; }");
    generatePermutations(0, size, array);
    ui->label_3->setText("Программа завершена.");
    ui->label_3->setStyleSheet("QLabel {color : green; }");
    count=0;
    inputString.clear();

}

void MainWindow::on_action_3_triggered()
{
    QMessageBox::about(this, tr("О программе"), tr("Данная программа выводит все перестановки\nзаданных N различных натуральных чисел"));
    return;
}

void MainWindow::on_action_4_triggered()
{
    QMessageBox::about(this, tr("Об авторе"), tr("Лабораторная работа №1\n\nВыполнил: Ковынев М.В.\nГруппа: 6304"));
    return;
}
