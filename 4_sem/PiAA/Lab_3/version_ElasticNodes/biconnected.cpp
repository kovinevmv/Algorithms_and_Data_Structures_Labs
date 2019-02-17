#include "biconnected.h"

vector<vector<int>>readmatrix(QString x)
{
    QString stringMatrix = x;
    int size=0;

    for (int i=0; i < stringMatrix.size(); ++i)
    {
        if (stringMatrix[i]=='[')
            ++size;
    }

    int j=0; int k=0;
    vector<vector<int>> matrix(size, vector<int>(size, 0));
    QString currentValue;

    for (int i=0;i<stringMatrix.size();++i)
    {
         if ((stringMatrix[i]!='[') &&
              (stringMatrix[i]!=']') &&
               (stringMatrix[i]!=' ') &&
                (stringMatrix[i]!='\n'))
         {
               if ((stringMatrix[i-1]=='[')||(stringMatrix[i-1]==' '))
                   currentValue=stringMatrix[i];
               else
                   currentValue+=stringMatrix[i];

               if ((stringMatrix[i+1]==']')||(stringMatrix[i+1]==' '))
               {
                   bool checkCorretConvert;
                   int curValue=currentValue.toInt(&checkCorretConvert,10);

                   if (checkCorretConvert)
                       matrix[k][j]=curValue;

                   if (j==size-1)
                   {
                       j=0;
                       ++k;
                   }
                   else
                       ++j;
               }
           }
       }
    return matrix;
}

QString pathfinder(int **F, int N, int **next, int **d)
{
    QString x;
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
    return x;

}

void convertMatrix(QString l, int **L, int N)
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

