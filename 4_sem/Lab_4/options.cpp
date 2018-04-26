#include "options.h"
#include "ui_options.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mscene.h"

Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);
}

Options::~Options()
{
    delete ui;
}

void Options::on_buttonBox_accepted()
{
    ptr->ui->graphicsView->getMscene()->setDiametr(ui->horizontalSlider->value());
    ptr->ui->graphicsView->getMscene()->setArrowSize(ui->horizontalSlider_2->value());

    if(ui->or_graph_yes->isChecked())
    {
        ptr->ui->graphicsView->getMscene()->setOrientated(true);
    }
    else
    {
        ptr->ui->graphicsView->getMscene()->setOrientated(false);
    }

    if(ui->weight_graph_yes->isChecked())
    {
        ptr->ui->graphicsView->getMscene()->setShowEdge(true);
    }
    else
    {
        ptr->ui->graphicsView->getMscene()->setShowEdge(false);
    }
}

void Options::on_buttonBox_rejected()
{

}
