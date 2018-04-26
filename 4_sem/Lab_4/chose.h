#ifndef CHOSE_H
#define CHOSE_H

#include <QDialog>

namespace Ui {
class chose;
}

class chose : public QDialog
{
    Q_OBJECT

public:
    explicit chose(QWidget *parent = 0);
    ~chose();

    QString* start_name;
    QString* finish_name;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::chose *ui;
};

#endif // CHOSE_H
