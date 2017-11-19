#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon("./24.jpg"));
    //setFixedSize(457,290);

    setWindowTitle("Лабораторная работа №2");
    ui->outputText->setReadOnly(true);
    data = new data_container;
    ui->inputText->setPlaceholderText("Пример: (a ((bc) 2 (def)))");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete data;
}


void MainWindow::generateComboBox()
{
    QString str = ui->inputText->toPlainText().simplified();
    ui->comboBox->clear();
    QStringList list = str.split(QRegExp("\\b"));
    for (int i=0; i<list.size(); i++)
        {
        QString x = list[i];
        if (!(x.contains("(") || x.contains(")") || x.contains(" ") ))
             ui->comboBox->addItem(list[i]);
        }

}

void MainWindow::on_inputText_textChanged()
{
    data->inp_str = ui->inputText->toPlainText().simplified();

    if (data->brack_analizator())
    {
        ui->label_3->setText("Введенные данные корректны!");
        ui->label_3->setStyleSheet("QLabel {color : green; }");
        ui->comboBox->setEnabled(true);
        ui->actionButton->setEnabled(true);
        ui->change_to->setEnabled(true);
        ui->label->setEnabled(true);
        ui->label_2->setEnabled(true);
        print(0);
    }
    else
    {
        ui->label_3->setText("Введенные данные некорректны! Измените входную строку.");
        ui->label_3->setStyleSheet("QLabel {color : red; }");
        ui->comboBox->setDisabled(true);
        ui->actionButton->setDisabled(true);
        ui->change_to->setDisabled(true);
        ui->label->setDisabled(true);
        ui->label_2->setDisabled(true);
    }
}

void MainWindow::print(bool a)
{
     ui->outputText->clear();
    data->out_str.clear();
    if (!a)
        generateComboBox();
    data->destroy(data->main_list);
    data->add_spaces_and_simplifie();
    data->str_in.setString(&data->inp_str);
    data->read_lisp(data->main_list);
    if (a)
        data->change_lisp(data->main_list,ui->comboBox->itemText(ui->comboBox->currentIndex()).simplified(), ui->change_to->toPlainText());
    data->write_lisp(data->main_list);
    data->out_str = data->out_str.simplified();
    ui->outputText->setPlainText(data->out_str);
}
void MainWindow::on_actionButton_clicked()
{
   print(1);
}

void MainWindow::on_action_3_triggered()
{
    QMessageBox::about(this, tr("О программе"), tr("Данная программа заменяет в иерархическом списке \nвсе вхождения заданного элемента (атома) x на заданный элемент (атом) y"));
    return;
}

void MainWindow::on_action_4_triggered()
{
    QMessageBox::about(this, tr("Об авторе"), tr("Лабораторная работа №2\n\nВыполнил: Ковынев М.В.\nГруппа: 6304"));
    return;
}

void MainWindow::on_action_triggered()
{
    QString FileName = QFileDialog::getOpenFileName(this, "Открыть файл...", QDir::homePath(), "Text files (*.txt)");
    if (FileName.isEmpty())
        return;

    QFile File(FileName);

    if(File.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->inputText->setPlainText(File.readAll());
        File.close();
    }
    else
    {
        QMessageBox::warning(this, "Error", QString("Не смог открыть файл %1").arg(FileName),QMessageBox::Ok);
    }
}

void MainWindow::on_action_2_triggered()
{
    if(ui->outputText->toPlainText().size() == 0)
        return;
    QString FileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"),QDir::homePath(),
                                                    tr("Text Files (*.txt)"));
    if (FileName.isEmpty())
        return;
    QFile File(FileName);
    if(File.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QByteArray Data;
        Data.append(ui->outputText->toPlainText());
        File.write(Data);
        File.close();
    }
}
