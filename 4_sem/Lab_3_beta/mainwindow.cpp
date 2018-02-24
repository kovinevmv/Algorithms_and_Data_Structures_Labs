#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <stack>
#include <cmath>
#include <QTime>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDebug>

static const double PI = 3.14159265358979323846264338327950288419717;
int NN;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString l=ui->plainTextEdit->toPlainText();
    if (l.size()>10)
    {
        int N=0;
        for (int i=0;i<l.size();++i)
        {
            if (l[i]=='[')
                ++N;
        }
        int **F = new int* [N];
        for (int i = 0; i < N; ++i)
            F[i] = new int [N];
        NN= N;
         int* array = new int [NN];
         for (int i=0; i<NN; i++)
             array[i] = -1;
        conv(l,F,N);
        pnt(F,N, array);
        for (int i = 0; i < N; ++i)
                        delete []F[i];

    }

}

void MainWindow::conv(QString l, int **L, int N)
{
    QString x;
    int j=0; int k=0;
    for (int i=0;i<l.size();++i)
    {
        if ((l[i]!='[')&&(l[i]!=']')&&(l[i]!=' ')&&(l[i]!='\n'))
        {
            if ((l[i-1]=='[')||(l[i-1]==' '))
            {
                x=l[i];
            }
            else
            {
                x+=l[i];
            }
            if ((l[i+1]==']')||(l[i+1]==' '))
            {
                bool ok;
                int n=x.toInt(&ok,10);
                if (ok)
                    L[k][j]=n;
                if (j==N-1)
                {
                    j=0;
                    ++k;
                }
                else
                    ++j;
            }
        }
    }
}

void MainWindow::pnt(int **F, int N, int* array)
{

    QRadialGradient gradient(-3, -3, 10);
    gradient.setColorAt(0, QColor(255, 104, 0));
    QPen blackpen(Qt::black);
    QBrush cyanBrush(gradient);
    QBrush cc(Qt::green);
    blackpen.setWidth(1);

    int **L = new int* [N];
    for (int i = 0; i < N; ++i)
        L[i] = new int [2];

    const float PI = 3.14159;
    float angle=360/N;
    angle = angle*PI/180;
    float r=20;
    scene->clear();


    float x=0;
    float y=-N*r;
    float oldx,oldy;
    for (int i=0;i<N;++i)
    {
        L[i][0]=x;
        L[i][1]=y;
        oldx = x;
        oldy = y;
        x=oldx*cos(angle)+oldy*sin(angle);
        y=-oldx*sin(angle)+oldy*cos(angle);
    }
    for (int i=0;i<N;++i)
    {
        for (int j=0;j<N;++j)
        {
            if (F[i][j]>0)
            {
                line = scene->addLine(L[i][0]+10,L[i][1]+10,L[j][0]+10,L[j][1]+10,blackpen);
                int x1,y1,x2,y2;
                x1=L[i][0]+10;
                x2=L[j][0]+10;
                y1=L[i][1]+10;
                y2=L[j][1]+10;
                obr(x1,y1,x2,y2,F[i][j]);
            }
        }
    }

    for (int i=1; i< array[0] -1 ; i++)
    {
        qDebug() << array[i] << " " << array[i+1];
        if (array[i] >= 0 && array[i] <=20)
        {
            int ii = array[i];
            int jj = array[i+1];
            QPen blackpen1(Qt::green);
            line = scene->addLine(L[ii][0]+10,L[ii][1]+10,L[jj][0]+10,L[jj][1]+10,blackpen1);
            int x1,y1,x2,y2;
            x1=L[ii][0]+10;
            x2=L[jj][0]+10;
            y1=L[ii][1]+10;
            y2=L[jj][1]+10;
            obr(x1,y1,x2,y2,F[ii][jj]);
        }

    }

    for (int i=0;i<N;++i)
    {
       ellipse = scene->addEllipse(L[i][0],L[i][1],20,20,blackpen,cyanBrush);
    }

    for (int i = 1; i < array[0]; i++)
        ellipse = scene->addEllipse(L[array[i]][0],L[array[i]][1],20,20,blackpen,cc);

    for (int i=0;i<N;++i)
    {
        QGraphicsTextItem *txt = new QGraphicsTextItem(QString::number(i+1));
        txt->setPos(L[i][0]+2, L[i][1]);
        scene->addItem(txt);
    }

    for (int i = 0; i < N; ++i)
        delete []L[i];

}

void MainWindow::obr(float x1, float y1, float x2, float y2, int ves)
{

    QPen blackpen(Qt::black);
    blackpen.setWidth(1);
    float x0,y0,x,y,xn,yn;

    float a=30;
    a = a*PI/180;
    x0=(x1+x2)/2;
    y0=(y1+y2)/2;
    float k;
    float b;
    if (x1!=x2)
    {
    k=(y2-y1)/(x2-x1);
    b=y1-k*x1;
    x=10*cos(atan(k));
    x=abs(x);
    if (x1>x2)
        x=x+x0;
    else
        x=x0-x;
    y=k*x+b;

    }
    else
    {
        x=x0;
        if (y1>y2)
            y=y0+10;
        else
            y=y0-10;
    }

    xn=x0-sin(a)*(y-y0)+cos(a)*(x-x0);
    yn=y0+cos(a)*(y-y0)+sin(a)*(x-x0);
    line = scene->addLine(x0,y0,xn,yn,blackpen);

    a=-30;
    a = a*PI/180;
    xn=x0-sin(a)*(y-y0)+cos(a)*(x-x0);
    yn=y0+cos(a)*(y-y0)+sin(a)*(x-x0);
    line = scene->addLine(x0,y0,xn,yn,blackpen);


    QGraphicsTextItem *txt = new QGraphicsTextItem(QString::number(ves));
        txt->setPos(x, y);
        scene->addItem(txt);

}

void MainWindow::on_pushButton_2_clicked()
{
    int qq=1;
    int* array = new int [NN];
    QString l1,l2;
    l1=ui->lineEdit->text();
    l2=ui->lineEdit_2->text();
    QString l=ui->plainTextEdit->toPlainText();
    if (l.size()>10)
    {

        int N=0;
        for (int i=0;i<l.size();++i)
        {
            if (l[i]=='[')
                ++N;
        }
        int **F = new int* [N];
        for (int i = 0; i < N; ++i)
            F[i] = new int [N];
        int **next = new int* [N];
        for (int i = 0; i < N; ++i)
            next[i] = new int [N];
        int **d = new int* [N];
        for (int i = 0; i < N; ++i)
            d[i] = new int [N];
        conv(l,F,N);
        QString path = "|";
        pathfinder(F,  N, next, d);
        if ((l1.size()>0)&&(l2.size()>0))
        {
            int u,v;
            bool ok1;
            int n=l1.toInt(&ok1,10) -1 ;
            if (ok1)
                u=n;
            n=l2.toInt(&ok1,10) - 1;
            if (ok1)
                v=n;
            int c;
            if (d[u][v] == 0)
                    ui->lineEdit_3->setText("No path found");
            else
            {
                c = u;
                while (c != v)
                {
                    if (c==u)
                    {
                        path=QString::number(c+1);
                        array[qq] = c;
                        qq++;
                    }
                    else
                    {
                        path+=QString::number(c+1);
                        array[qq] = c;
                        qq++;
                    }

                        path+="->";

                    c = next[c][v];
                }
                path+=QString::number(v+1);
                 array[qq] = v;
                 qq++;
                path+='=';
                path+=QString::number(d[u][v]);
                ui->lineEdit_3->setText(path);
            }
        }

        array[0] = qq;

        pnt(F,N, array);
        for (int i = 0; i < N; ++i)
                        delete []d[i];
        for (int i = 0; i < N; ++i)
                        delete []next[i];
        for (int i = 0; i < N; ++i)
                        delete []F[i];


    }
    else
        ui->plainTextEdit_2->setPlainText("error");
}

void MainWindow::pathfinder(int **F, int N, int **next, int **d)
{

    for (int i=0;i<N;++i)
    {
        for (int j=0;j<N;++j)
        {
            if (F[i][j]!=0)
                next[i][j]=j;
            else
                next[i][j]=0;
        }
    }

    for (int i=0;i<N;++i)
    {
        for (int j=0;j<N;++j)
        {
            d[i][j]=F[i][j];
            if (F[i][j]==0)
                d[i][j]=1000;
        }
    }
    for (int i=0;i<N;++i)
    {
        for (int u=0;u<N;++u)
        {
            for (int v=0;v<N;++v)
            {
                if (d[u][i] + d[i][v] < d[u][v])
                {
                    d[u][v] = d[u][i] + d[i][v];
                    next[u][v] = next[u][i];
                }
            }
        }
    }
    QString x;
    for (int i=0;i<N;++i)
    {
        for (int j=0;j<N;++j)
        {
            if ((i==0)&&(j==0))
                x='[';
            else
                if (j==0)
                    x+='[';
            if (d[i][j]==1000)
                d[i][j]=0;
            if (i==j)
                d[i][j]=0;
            x+=QString::number(d[i][j]);
            if (j==N-1)
            {
                x+=']';
                x+='\n';
            }
            else
                x+=' ';
        }
    }
    ui->plainTextEdit_2->setPlainText(x);
}

void MainWindow::gen(int **F,int N)
{
    for (int i=0;i<N;++i)
    {
        for (int j=0;j<N;++j)
        {
            F[i][j]=0;
        }
    }
    for (int i=0;i<N;++i)
    {
        int n=0;
        while (n==0)
            n=qrand() % 3;
        for (int j=0;j<n;++j)
        {
            int p=qrand() % N;
            int v=0;
            while (v==0)
                 v=qrand() % 30;
            F[i][p]=v;
        }
    }
}

void MainWindow::on_action_2_triggered()
{
    int N=0;
    while (N<5)
        N=qrand() % 20;
    int **F = new int* [N];
    for (int i = 0; i < N; ++i)
        F[i] = new int [N];
    gen (F,N);
    QString x;
    for (int i=0;i<N;++i)
    {
        for (int j=0;j<N;++j)
        {
            if ((i==0)&&(j==0))
                x='[';
            else
                if (j==0)
                    x+='[';
            x+=QString::number(F[i][j]);
            if (j==N-1)
            {
                x+=']';
                x+='\n';
            }
            else
                x+=' ';
        }
    }
    ui->plainTextEdit->setPlainText(x);

    for (int i = 0; i < N; ++i)
                    delete []F[i];
}
