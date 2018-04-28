#include "chose.h"
#include "ui_chose.h"

chose::chose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chose)
{
    ui->setupUi(this);
}

chose::~chose()
{
    delete ui;
}

void chose::on_buttonBox_accepted()
{
    *start_name = ui->lineEdit->text();
    *finish_name = ui->lineEdit_2->text();
}

void chose::on_buttonBox_rejected()
{
    *start_name=*finish_name="\0";
}
