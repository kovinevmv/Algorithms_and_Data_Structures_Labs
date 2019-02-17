#ifndef HELPBROWSER_H
#define HELPBROWSER_H
#include <QtWidgets>

class InformationDialog : public QWidget{
	Q_OBJECT

public:

    // Создание окна справки
	InformationDialog(const QString& strPath,const QString& strFileName, QWidget* pwgt       = 0 );

    // Создания окна текстового отображения
	InformationDialog(const QString& text_view, QWidget *par);

    ~InformationDialog(){}

private:

	QString name;
    void readSettings();   // Сохранение истории посещений
	void writeSettings();

protected:
	void closeEvent(QCloseEvent * event);
};

#endif // HELPBROWSER_H
