#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QWidget>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>

using namespace binTree_modul;

binTree enterBT(QStringList string);
void displayBT(binTree b, int n);
void levelBT(binTree b);

void MainWindow::clear_tree()
{
    ui->label1_1->setVisible(0);
    ui->label1_2->setVisible(0);
    ui->label_0->setVisible(0);
    ui->label_2->setVisible(0);
    ui->label2_1->setVisible(0);
    ui->label2_2->setVisible(0);
    ui->label_3->setVisible(0);
    ui->label3_1->setVisible(0);
    ui->label3_2->setVisible(0);
    ui->label4_1->setVisible(0);
    ui->label4_2->setVisible(0);
    ui->label4_3->setVisible(0);
    ui->label4_4->setVisible(0);

    ui->label_name_1->setVisible(0);
    ui->label_name_2->setVisible(0);
    ui->label_name_3->setVisible(0);
    ui->label_name_4->setVisible(0);
    ui->label_name_5->setVisible(0);
    ui->label_name_6->setVisible(0);
    ui->label_name_7->setVisible(0);

    window()->setFixedSize(340, 340);


    ui->pushButton->setEnabled(0);
    ui->pushButton_2->setEnabled(0);
    ui->comboBox->setEnabled(0);
    ui->textEdit_2->setEnabled(0);

    ui->comboBox->clear();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

   ui->setupUi(this);

   ui->textEdit->setPlaceholderText("Пример: Я Мама Бабушка / / Дедушка / /  Папа  Бабушка / / Дедушка / / ");

   QPixmap picture_root("D:\\tree_round.png");
   QPixmap picture_line_1("D:\\tree_line.png");
   QPixmap picture_line_2("D:\\tree_line_2.png");
   QPixmap picture_line_rr_2("D:\\line_rr_1.png");
   QPixmap picture_line_rr_1("D:\\line_rr_2.png");

   ui->label_0->setPixmap(picture_root);
   ui->label1_1->setPixmap(picture_line_1);
   ui->label_2->setPixmap(picture_root);
   ui->label1_2->setPixmap(picture_line_2);
   ui->label_3->setPixmap(picture_root);
   ui->label2_1->setPixmap(picture_line_rr_1);
   ui->label2_2->setPixmap(picture_line_rr_2);


   ui->label3_1->setPixmap(picture_line_rr_1);
   ui->label3_2->setPixmap(picture_line_rr_2);

   ui->label4_1->setPixmap(picture_root);
   ui->label4_2->setPixmap(picture_root);
   ui->label4_3->setPixmap(picture_root);
   ui->label4_4->setPixmap(picture_root);
   clear_tree();




}

MainWindow::~MainWindow()
{
    delete ui;
}


binTree b;
int index=0;
binTree enterBT(QStringList string)
{
    QString ch;
    binTree p, q;
    ch = string[index];
    index++;
    if (ch == "/")
        return NULL;
    else
    {
        p = enterBT(string);
        q = enterBT(string);
        return ConsBT(ch, p, q);
    }

}

void MainWindow::on_pushButton_clicked()
{
    window()->setFixedSize(765, 350);

    int val=0;
    if (!isNull(b))
    {
        val++;
         ui->label_0->setVisible(1);
         ui->label_name_1->setVisible(1);
         ui->label_name_1->setText(RootBT(b));
    }

    if (!isNull(Left(b)))
    {
        val++;
         ui->label1_1->setVisible(1);
         ui->label_2->setVisible(1);
         ui->label_name_2->setVisible(1);
         ui->label_name_2->setText(RootBT(Left(b)));
    }

    if (!isNull(Right(b)))
    {
        val++;
        ui->label1_2->setVisible(1);
        ui->label_3->setVisible(1);
        ui->label_name_3->setVisible(1);
        ui->label_name_3->setText(RootBT(Right(b)));
    }


    if (!isNull(Left(Left(b))))
    {
        val++;
        ui->label4_1->setVisible(1);
        ui->label2_1->setVisible(1);
        ui->label_name_4->setVisible(1);
        ui->label_name_4->setText(RootBT(Left(Left(b))));
    }

    if (!isNull(Right(Left(b))))
    {
        val++;
        ui->label4_2->setVisible(1);
        ui->label2_2->setVisible(1);
        ui->label_name_5->setVisible(1);
        ui->label_name_5->setText(RootBT(Right(Left(b))));
    }

    if (!isNull(Left(Right(b))))
    {
        val++;
        ui->label4_3->setVisible(1);
        ui->label3_1->setVisible(1);
        ui->label_name_6->setVisible(1);
        ui->label_name_6->setText(RootBT(Left(Right(b))));
    }

    if (!isNull(Right(Right(b))))
    {
        val++;
        ui->label4_4->setVisible(1);
        ui->label3_2->setVisible(1);
        ui->label_name_7->setVisible(1);
        ui->label_name_7->setText(RootBT(Right(Right(b))));
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

void MainWindow::on_action_3_triggered()
{
    QMessageBox::about(this, tr("О программе"), tr("Задача: Задано бинарное дерево b типа ВТ с типом элементов Elem. Для введенной пользователем величины Е (var E: Elem) определить число вхождений элемента Е в дерево b."));
    return;
}

void MainWindow::on_action_4_triggered()
{
    QMessageBox::about(this, tr("Об авторе"), tr("Лабораторная работа №4\n\nВыполнил: Ковынев М.В.\nГруппа: 6304"));
    return;
}



bool isCorrect(QStringList list)
{
    int br=0;
    int val=0;
    for (int i=0; i< list.size(); i++)
    {
        if (list[i]=="/")
            br++;
        else val++;
    }
    return (val+1==br);
}

void MainWindow::on_textEdit_textChanged()
{
    clear_tree();
    QString string = ui->textEdit->toPlainText();
    QStringList list = string.split(QRegExp("\\s"), QString::SkipEmptyParts);
    if (isCorrect(list))
    {
        ui->warningl->setText("Введенное представление дерево корректно!");
        ui->warningl->setStyleSheet("QLabel {color : green; }");
        ui->pushButton->setEnabled(1);
        ui->pushButton_2->setEnabled(1);
        ui->comboBox->setEnabled(1);
        ui->textEdit_2->setEnabled(1);

    }
    else
    {
        ui->warningl->setText("Введенное представление дерево некорректно!");
        ui->warningl->setStyleSheet("QLabel {color : red; }");
        ui->pushButton->setEnabled(0);
        ui->pushButton_2->setEnabled(0);
        ui->comboBox->setEnabled(0);
        ui->textEdit_2->setEnabled(0);
    }
}

int HeightOfTree(binTree b)
{
    if(isNull(b))
        return 0;
    int left, right;
    if (!isNull(Left(b)))
    {
        left = HeightOfTree(Left(b));
    }
    else
        left = -1;

    if (!isNull(Right(b)))
    {
        right = HeightOfTree(Right(b));
    }
    else
        right = -1;
    int max = left > right ? left : right;
    return max+1;

}

void MainWindow::on_pushButton_2_clicked()
{
    ui->comboBox->clear();
    index=0;
    QString string = ui->textEdit->toPlainText();
    QStringList list = string.split(QRegExp("\\s"), QString::SkipEmptyParts);

    b = enterBT(list);
    if (HeightOfTree(b) > 2)
    {
        ui->pushButton->setDisabled(1);
        ui->warningl->setText("Высота дерева больше 3. Отображение невозможно.");
        ui->warningl->setStyleSheet("QLabel {color : red; }");
    }

    for(int i = 0; i < list.size(); i++)
    {
        bool b = true;
        for(int j = i + 1; j < list.size(); j++)
        {
            if(list[i] == list[j])
                b = false;
        }
        if(b)
        {
            if (list[i] !="/")
            {

                ui->comboBox->addItem(list[i]);
            }

        }
    }


}

int counter=0;
void go(binTree b, QString str)
{
    if (!isNull(Left(b))) go(Left(b), str);
    if (!isNull(Right(b))) go(Right(b), str);
    if (RootBT(b) == str)
        counter++;
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    counter=0;
    go(b, ui->comboBox->currentText());
    ui->textEdit_2->setText("Найдено элементов (" +ui->comboBox->currentText()+"): "+QString::number(counter));
}

void MainWindow::on_radioButton_clicked()
{
    srand(time(NULL));
    QStringList list = {"Я Мама Бабушка / / Дедушка / /  Папа  Бабушка / / Дедушка / /", "Qt / is best / / program / /", "Корень Узел Лист / / Лист / / Лист / /", "ОдинЭлемент / /"};
    ui->textEdit->setText(list[rand()%list.size()]);
}
