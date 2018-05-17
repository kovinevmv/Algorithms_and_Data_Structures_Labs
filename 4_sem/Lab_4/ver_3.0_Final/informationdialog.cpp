#include "informationdialog.h"
#include "graphwindow.h"
InformationDialog::InformationDialog(const QString& strPath,const QString& strFileName, QWidget* pwgt)
	: QWidget(pwgt)
{
	setAttribute(Qt::WA_DeleteOnClose); //удаление при закрытии

	setWindowFlags(Qt::Window |Qt::CustomizeWindowHint |Qt::WindowMinMaxButtonsHint| Qt::WindowCloseButtonHint);

	name = "browser";
	readSettings();

	setWindowModality(Qt::WindowModal);
	setWindowIcon(QIcon(":/icons/help.png"));
	setWindowTitle("Справка");

	//кнопки для навигации по справке
	QPushButton*  pcmdBack    = new QPushButton("<<");
	QPushButton*  pcmdHome    = new QPushButton("Главная");
	QPushButton*  pcmdForward = new QPushButton(">>");

	QTextBrowser* ptxtBrowser = new QTextBrowser; //браузер

	connect(pcmdBack, &QPushButton::clicked, ptxtBrowser, &QTextBrowser::backward);
	connect(pcmdHome, &QPushButton::clicked, ptxtBrowser, &QTextBrowser::home);
	connect(pcmdForward, &QPushButton::clicked, ptxtBrowser, &QTextBrowser::forward);
	connect(ptxtBrowser, &QTextBrowser::backwardAvailable, pcmdBack,&QPushButton::setEnabled);
	connect(ptxtBrowser, &QTextBrowser::forwardAvailable, pcmdForward, &QPushButton::setEnabled);

	ptxtBrowser->setSearchPaths(QStringList() << strPath);
    ptxtBrowser->setSource(QString("1.htm"));
	ptxtBrowser->setSource(strFileName);
	//Layout setup
	QVBoxLayout* pvbxLayout = new QVBoxLayout;
	QHBoxLayout* phbxLayout = new QHBoxLayout;
	phbxLayout->addWidget(pcmdBack);
	phbxLayout->addWidget(pcmdHome);
	phbxLayout->addWidget(pcmdForward);
	pvbxLayout->addLayout(phbxLayout);
	pvbxLayout->addWidget(ptxtBrowser);
	setLayout(pvbxLayout);
}

//текстовое отображение
InformationDialog::InformationDialog(const QString &text_view,QWidget * par)
	:QWidget(par)
{
	setWindowModality(Qt::ApplicationModal);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowFlags(Qt::Window |Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
	setWindowIcon(QIcon(":/icons/graph.png"));
    //setFixedSize(300,400);

	name = "text_view";
	readSettings();

    setWindowTitle("Информация");

	QPushButton * saveButton = new QPushButton("Сохранить");
	QPushButton * rejectButton = new QPushButton("Отмена");
	QPlainTextEdit * browser = new QPlainTextEdit;
	browser->setReadOnly(true);
	browser->setPlainText(text_view);

	GraphWindow * p = dynamic_cast<GraphWindow*>(par); //родитель должен быть типа GraphWindow!

	connect(saveButton,&QPushButton::clicked,[this,p]() //вопрос о сохранении графа
												{
													int b = QMessageBox::question(this,"Сохранить...","Сохранить граф с форматированием?",QMessageBox::Yes , QMessageBox::No,QMessageBox::Cancel);
													if(b==QMessageBox::Yes)
														p->saveGraphWithFormat();
													else if(b==QMessageBox::No)
														p->saveTextGraph();
												});
	connect(rejectButton,&QPushButton::clicked,this,&InformationDialog::close);

	QGridLayout * lay = new QGridLayout;
	lay->addWidget(browser,0,0,1,2);
	lay->addWidget(saveButton,1,0);
	lay->addWidget(rejectButton,1,1);
	setLayout(lay);
}


void InformationDialog::writeSettings()
{
	QSettings settings("LETI", "Graph application");
	settings.setValue(name,saveGeometry());
}

//установка настроек
void InformationDialog::readSettings()
{
	QSettings settings("LETI", "Graph application");
	restoreGeometry(settings.value(name).toByteArray());
}


void InformationDialog::closeEvent(QCloseEvent *event)
{
	writeSettings();
	event->accept();
}

