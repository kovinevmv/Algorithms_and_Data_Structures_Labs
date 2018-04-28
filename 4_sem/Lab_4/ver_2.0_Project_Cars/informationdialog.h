#ifndef HELPBROWSER_H
#define HELPBROWSER_H
#include <QtWidgets>

class InformationDialog : public QWidget{
	Q_OBJECT

public:
	//для окна справки
	InformationDialog(const QString& strPath,const QString& strFileName, QWidget* pwgt       = 0 );

	//для окна текстового отображения
	InformationDialog(const QString& text_view, QWidget *par);

	~InformationDialog(){qDebug () << "help deleted";}

private:
	QString name;
	void readSettings();
	void writeSettings();

protected:
	void closeEvent(QCloseEvent * event);
};

#endif // HELPBROWSER_H
