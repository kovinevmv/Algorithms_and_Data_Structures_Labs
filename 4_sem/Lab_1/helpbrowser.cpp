#include "helpbrowser.h"

HelpBrowser::HelpBrowser(const QString& strPath,
            const QString& strFileName,
                  QWidget* pwgt
           ) : QDialog(pwgt)
{
    //окно со справкой на базе браузера
    setWindowFlags(Qt::Dialog);
    setWindowFlags(Qt::CustomizeWindowHint |Qt::WindowMinMaxButtonsHint| Qt::WindowCloseButtonHint);
    setMinimumSize(700,400);
    move(200,100);
    setWindowModality(Qt::WindowModal);
    setWindowIcon(QIcon(":/images/images/main.png"));
    setWindowTitle("Справка");

    QPushButton*  pcmdBack    = new QPushButton("Предыдущая страница");
    QPushButton*  pcmdHome    = new QPushButton("Справка");
    QPushButton*  pcmdForward = new QPushButton("Следующая страница");
    QTextBrowser* ptxtBrowser = new QTextBrowser;

    connect(pcmdBack, SIGNAL(clicked()), ptxtBrowser, SLOT(backward()));
    connect(pcmdHome, SIGNAL(clicked()), ptxtBrowser, SLOT(home()));
    connect(pcmdForward, SIGNAL(clicked()), ptxtBrowser, SLOT(forward()));
    connect(ptxtBrowser, SIGNAL(backwardAvailable(bool)), pcmdBack, SLOT(setEnabled(bool)));
    connect(ptxtBrowser, SIGNAL(forwardAvailable(bool)), pcmdForward, SLOT(setEnabled(bool)));

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

    name = "faq";
    readSettings();

}


//окно с текстовым представлением
HelpBrowser::HelpBrowser(const QString &text_view)
{
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    setWindowIcon(QIcon(":/images/images/graph.png"));
    setFixedSize(300,400);

    QPushButton * saveButton = new QPushButton("Сохранить");
//сохранение при данной кнопке
    QPushButton * rejectButton = new QPushButton("Отмена");
    QPlainTextEdit * browser = new QPlainTextEdit;
    browser->setReadOnly(true);
    browser->setPlainText(text_view);

    connect(saveButton,SIGNAL(clicked(bool)),SLOT(accept()));
    connect(rejectButton,SIGNAL(clicked(bool)),SLOT(reject()));

    QGridLayout * lay = new QGridLayout;
    lay->addWidget(browser,0,0,1,2);
    lay->addWidget(saveButton,1,0);
    lay->addWidget(rejectButton,1,1);
    setLayout(lay);

    name = "text_view";
    readSettings();
}

//фукнция для вывода окна
bool HelpBrowser::outDialog(const QString &text, QString title)
{
    HelpBrowser * temp = new HelpBrowser(text);
    temp->setWindowTitle(title);
    temp->show();
    if(temp->exec() == QDialog::Accepted)
    {
        delete temp;
        return true;

    }
    delete temp;
    return false;
}

//применить и закрыть
void HelpBrowser::ok_changes()
{
    appl_changes();
    this->close();
}

//применить настройки
void HelpBrowser::appl_changes()
{
}

void HelpBrowser::writeSettings()
{
    QSettings settings("LETI", "Graph application");
    settings.setValue(name,saveGeometry());
}

//установка настроек
void HelpBrowser::readSettings()
{
    QSettings settings("LETI", "Graph application");
    restoreGeometry(settings.value(name).toByteArray());
}

void HelpBrowser::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}
