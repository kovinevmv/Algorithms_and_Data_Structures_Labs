#ifndef HELPBROWSER_H
#define HELPBROWSER_H
#include <QtWidgets>
//класс для отображения текстового представления
//справки
//или окна настроек
class HelpBrowser : public QDialog{
    Q_OBJECT

public:
    HelpBrowser(const QString& strPath,const QString& strFileName, QWidget* pwgt       = 0 );
//конструктор для окна справки

   static bool outDialog(const QString& text, QString title);
//функция для вызова текстового представления

private:
        HelpBrowser(const QString& text_view);
//конструктор для окна текстового представления

        int new_diam; //параметры для окна настройки
        int arr_size;
        bool oriented;

        QGraphicsScene* scene_for_set;

        QString name;
        void readSettings();
        void writeSettings();
private slots:
    void set_new_diam(int a){new_diam = a;} //установка настроек
    void set_oriented(bool b){oriented = b;}
    void set_new_arr(int a){arr_size = a;}
    void ok_changes();
    void appl_changes();
  //  void appl_changes(); //применить настройки

protected:
    void closeEvent(QCloseEvent * event);
};

#endif // HELPBROWSER_H
