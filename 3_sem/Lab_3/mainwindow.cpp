#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <stack>
#include <string>
#include <fstream>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QString>
#include <algorithm>
using namespace std;

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

void MainWindow::on_action_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"), QString(),
                tr("Text Files (*.txt)"));

        if (!fileName.isEmpty())
        {
            QFile file(fileName);
            if (!file.open(QIODevice::ReadOnly))
            {
                QMessageBox::critical(this, tr("Ошибка"), tr("Невозможно открыть файл!"));
                return;
            }
            QTextStream in(&file);
            ui->textEdit->setText(in.readAll());
            file.close();
        }
         ui->radioButton_3->setChecked(true);
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
               QMessageBox::critical(this, tr("Ошибка"), tr("Невозможно сохранить файл!"));
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

void MainWindow::on_pushButton_2_clicked()
{
    exit(1);
}



void MainWindow::on_pushButton_clicked()
{
   ui->textEdit_2->clear();
   QString b = ui->textEdit->toPlainText();

   stack <QChar> st;

   int k=0;
   for (int i = 0; i < b.size(); i++)
   {
       if (b[i] != '\n')
       {
           st.push(b[i]);
           k++;
       }
       else
       {
           for (int j=0; j<k; j++)
           {
                ui->textEdit_2->setText(ui->textEdit_2->toPlainText() + st.top());
                st.pop();

           }
           ui->textEdit_2->setText(ui->textEdit_2->toPlainText() + "\n");
           k=0;
       }

   }
   for (int j=0; j<k; j++)
   {
        ui->textEdit_2->setText(ui->textEdit_2->toPlainText() + st.top());
        st.pop();
   }
    ui->textEdit_2->setText(ui->textEdit_2->toPlainText() + "\n");
}

void MainWindow::on_radioButton_clicked()
{
    ui->textEdit->clear();
    ui->textEdit_2->clear();
   ui->radioButton_2->setChecked(false);
   ui->radioButton_3->setChecked(false);
   ui->radioButton_4->setChecked(false);
   QString array[9] = { "Среда программирования Qt Creator (IDE QT Creator) находится в репозитории большинства современных дистрибутивов Linux",
                        "Последнюю версию IDE Qt Creator можно скачать на сайте QtProject (http://qt-project.org/downloads).",
                        "Рассмотрим простейшие приёмы работы в среде Qt Creator на примере создания консольного приложения для решения задачи.",
                        "Для создания простейшего консольного приложения выбираем Non-Qt Project.",
                        "Дальнейшее знакомство со средой Qt Creator продолжим, решая следующую задачу.",
                        "Заменим текст «Hello, Word» стандартного приложения, на текст программы решения задачи.",
                        "В меню File (Файл) выбрать команду New File or Project (Новый файл или проект) (комбинация клавиш Ctrl+N).",
                        "Далее выбираем имя проекта и каталог для его размещения.",
                        "Откомпилировать и запустить программу можно одним из следующих способов."};
   int len = rand()%6+3;
   for (int i=1; i<len; i++)
   {
        ui->textEdit->setText(ui->textEdit->toPlainText() + array[rand()%9]+ "\n");
   }


}

QString GetRandomString()
{
   QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ");
   int randomStringLength = rand()%20+5;
   QString randomString;
   for(int i=0; i<randomStringLength; ++i)
   {
       int index = rand() % possibleCharacters.length();
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
   randomString.append("\n");
   return randomString;
}
void MainWindow::on_radioButton_2_clicked()
{
    ui->textEdit->clear();
    ui->textEdit_2->clear();
    ui->radioButton->setChecked(false);
    ui->radioButton_3->setChecked(false);
    ui->radioButton_4->setChecked(false);
    int len = rand()%6+3;

    for (int i=1; i<len; i++)
    {
        ui->textEdit->setText(ui->textEdit->toPlainText() + GetRandomString());
    }

}

void MainWindow::on_radioButton_3_clicked()
{
    ui->textEdit->clear();
    ui->textEdit_2->clear();
    ui->radioButton->setChecked(false);
    ui->radioButton_2->setChecked(false);
    ui->radioButton_4->setChecked(false);
    on_action_triggered();
}

void MainWindow::on_radioButton_5_clicked()
{
    on_action_2_triggered();
    ui->radioButton_5->setChecked(false);
}

void MainWindow::on_radioButton_4_clicked()
{
    ui->textEdit->clear();
    ui->textEdit_2->clear();
    ui->radioButton->setChecked(false);
    ui->radioButton_2->setChecked(false);
    ui->radioButton_3->setChecked(false);
    ui->textEdit->setPlaceholderText(tr("Введите здесь свой текст"));

}

void MainWindow::on_action_3_triggered()
{
    QMessageBox::about(this, tr("О программе"), tr("Содержимое заданного текстового файла F, разделенного на строки, переписать в текстовый файл G, выписывая литеры каждой строки в обратном порядке. В решении задачи использовать стек."));
    return;
}

void MainWindow::on_action_4_triggered()
{
     QMessageBox::about(this, tr("Об авторе"), tr("Выполнил:  Ковынев М.В.\nГруппа: 6304"));
     return;
}
