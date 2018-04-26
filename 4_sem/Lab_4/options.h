#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>
#include "mainwindow.h"

class MainWindow;

namespace Ui {
class Options;
}

class Options : public QDialog
{
    friend class MainWindow;

    Q_OBJECT

public:
    explicit Options(QWidget *parent = 0);
    ~Options();
    MainWindow* ptr;
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::Options *ui;

};

#endif // OPTIONS_H
