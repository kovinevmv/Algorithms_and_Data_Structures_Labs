#include "solver.h"
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<cmath>
#include<algorithm>
#include<iostream>
#include<ctime>
#include <vector>

int width = 300;
int M = 20;

int factor = 0;
int index = 1;
int color = 0;
int f[2505];
int N;

bool a[55][55];
int r[55][55], d[55][55];
int changecolor = 1;
QProgressBar *progressBar;
QLabel *label;


// Установка цвета на квадраты
RGBPIXEL setcolor(int n)
{
    RGBPIXEL currentColor;
    if (changecolor == 0)
    {
        currentColor.rgbBlue = 255;
        currentColor.rgbGreen = 255;
        currentColor.rgbRed = 255;
        return currentColor;
    }
    else
    {

        switch (n)
        {
        case 0:  {currentColor.rgbRed = 230; currentColor.rgbGreen = 230; currentColor.rgbBlue = 255;   return (currentColor); break; }
        case 1:  {currentColor.rgbRed = 0;   currentColor.rgbGreen = 128; currentColor.rgbBlue = 128; return (currentColor); break; }
        case 2:  {currentColor.rgbRed = 211; currentColor.rgbGreen = 255; currentColor.rgbBlue = 206; return (currentColor); break; }
        case 3:  {currentColor.rgbRed = 255; currentColor.rgbGreen = 165; currentColor.rgbBlue = 0;   return (currentColor); break; }
        case 4:  {currentColor.rgbRed = 191; currentColor.rgbGreen = 214; currentColor.rgbBlue = 65; return (currentColor); break; }
        case 5:  {currentColor.rgbRed = 0;   currentColor.rgbGreen = 51;  currentColor.rgbBlue = 102; return (currentColor); break; }
        case 6:  {currentColor.rgbRed = 250; currentColor.rgbGreen = 128; currentColor.rgbBlue = 114; return (currentColor); break; }
        case 7:  {currentColor.rgbRed = 8;   currentColor.rgbGreen = 141; currentColor.rgbBlue = 165; return (currentColor); break; }
        case 8:  {currentColor.rgbRed = 13;  currentColor.rgbGreen = 54;  currentColor.rgbBlue = 66;  return (currentColor); break; }
        case 9:  {currentColor.rgbRed = 127; currentColor.rgbGreen = 127; currentColor.rgbBlue = 0;   return (currentColor); break; }
        case 10: {currentColor.rgbRed = 127; currentColor.rgbGreen = 0;   currentColor.rgbBlue = 127; return (currentColor); break; }
        case 11: {currentColor.rgbRed = 0;   currentColor.rgbGreen = 127; currentColor.rgbBlue = 127; return (currentColor); break; }
        case 12: {currentColor.rgbRed = 236; currentColor.rgbGreen = 219; currentColor.rgbBlue = 84;  return (currentColor); break; }
        case 13: {currentColor.rgbRed = 233; currentColor.rgbGreen = 75;  currentColor.rgbBlue = 60;  return (currentColor); break; }
        case 14: {currentColor.rgbRed = 111; currentColor.rgbGreen = 159; currentColor.rgbBlue = 216; return (currentColor); break; }
        case 15: {currentColor.rgbRed = 0;   currentColor.rgbGreen = 165; currentColor.rgbBlue = 145; return (currentColor); break; }
        case 16: {currentColor.rgbRed = 234; currentColor.rgbGreen = 222; currentColor.rgbBlue = 219; return (currentColor); break; }
        case 17: {currentColor.rgbRed = 255; currentColor.rgbGreen = 214; currentColor.rgbBlue = 0;  return (currentColor); break; }

        default: {currentColor.rgbRed = rand() % 255; currentColor.rgbGreen = rand() % 255; currentColor.rgbBlue = rand() % 255; return (currentColor); break; }
        }
    }

}


// Создание матрицы квадратов с цветом
void addColorToMatrix(squareCoord curCoord, vector<vector<RGBPIXEL>> &a)
{
    // Установка цвета
    RGBPIXEL n = setcolor(color);

    // Заливка области
    for (int i = factor*(curCoord.x - 1); i < factor * (curCoord.size + curCoord.x - 1); i++)
    {
        for (int j = factor*(curCoord.y - 1); j < factor * (curCoord.size + curCoord.y - 1); j++)
        {
            a[i][j] = n;
        }
    }


    // Черная рамка вокруг квадрата
    RGBPIXEL nn;
    nn.rgbBlue = 0;
    nn.rgbGreen = 0;
    nn.rgbRed = 0;
    for (int i = factor * (curCoord.x - 1); i < factor * (curCoord.size + curCoord.x - 1); i++)
    {
        a[i][factor * (curCoord.y - 1)] = nn;
        a[i][factor * (curCoord.size + curCoord.y - 1)-1] = nn;
    }
    for (int j = factor * (curCoord.y - 1); j < factor * (curCoord.size + curCoord.y - 1); j++)
    {
        a[factor * (curCoord.x - 1)][j] = nn;
        a[factor * (curCoord.size + curCoord.x - 1)-1][j] = nn;
    }

    color++;
}

void building(int x, int y, int size, vector<vector<RGBPIXEL>> &a)
{
    // Создание объекта квадрата
    // Занесение данных в структуру квадрата
    squareCoord temp;
    temp.x = x;
    temp.y = y;
    temp.size = size;
    addColorToMatrix(temp, a);
}


// Сохранение матрицы как картинку с расширением ppm
void saveAsPPM(vector<vector<RGBPIXEL>> &b, int i)
{
    char fileNameIndex[3];
    sprintf(fileNameIndex, "%d", i);
    char fileName[30] = "./output_";
    strcat(fileName, fileNameIndex);
    strcat(fileName, ".ppm");
    FILE *f = fopen(fileName, "wb");

    fprintf(f, "P6\n%i %i 255\n", N*factor, N*factor);
    for (int y = 0; y<N*factor; y++) {
        for (int x = 0; x<N*factor; x++) {
            fputc(b[x][y].rgbRed, f);   // 0 .. 255
            fputc(b[x][y].rgbGreen, f); // 0 .. 255
            fputc(b[x][y].rgbBlue, f);  // 0 .. 255
        }
    }
    fclose(f);
}


struct AS
{
    int len;
    int x, y;
} tmp[2505], ans[2505];

struct SS
{
    int y, high;
} stack[55];


inline void backtracking(int x, int y, int sum, int S, vector<vector<RGBPIXEL>> &b)
{
    color = 0;
    if (sum + f[S] >= M)
        return;

    if (x<N)
    {
        int tot = 0;
        for (int i = 1; i <= N; ++i)
            if (a[x + 1][i])
                i += r[x + 1][i];
            else
            {
                while (i<N && !a[x + 1][i])
                    ++i;
                ++tot;
            }
            if (sum + tot >= M)
                return;
    }
    if (x>N)
    {
        // Сохраняем новое приближение
        M = sum;
        memcpy(ans, tmp, sizeof(AS)*M);
        for (int i = 0; i<M; ++i)
            building(ans[i].x, ans[i].y, ans[i].len, b);
        saveAsPPM(b, index);
        progressBar->setValue(index*10);
        index++;
        return;
    }
    else if (y>N)
    {
        int i, top = 0, tot = 0, mid, s = 0, tmp;
        ++x;
        if (x <= N)
        {
            for (i = 1; i <= N; ++i)
                if (!a[x][i])
                {
                    while (i < N && !a[x][i + 1])++i;
                    {
                        struct SS n;
                        n.y = i;
                        n.high = 0;
                        stack[top++] = n;
                    }
                }
                else
                {
                    if (top&&stack[top - 1].high <= d[x][i])
                    {
                        mid = stack[--top].high;
                        if (top)
                            while (top&&stack[top - 1].high <= d[x][i])
                            {
                                --top;
                                if (i - stack[top].y - 1 <= stack[top].high - mid)
                                    tmp = (stack[top].high - mid) / (i - stack[top].y - 1);
                                else tmp = 0;
                                tot += tmp;
                                s += (stack[top].high - mid)*(i - stack[top].y - 1) -
                                    tmp * (i - stack[top].y - 1)*(i - stack[top].y - 1);
                                mid = stack[top].high;
                            }
                        else
                            if (d[x][i] != mid)
                            {
                                if (i - 1 <= d[x][i] - mid)tmp = (d[x][i] - mid) / (i - 1);
                                    else tmp = 0;
                                tot += tmp;
                                s += (d[x][i] - mid)*(i - 1) - tmp * (i - 1)*(i - 1);
                            }
                    }
                    struct SS n;
                    n.y = i + r[x][i] - 1;
                    n.high = d[x][i];
                    stack[top++] = n;
                    i += r[x][i] - 1;
                }

                mid = stack[top - 1].high;

                if (top)
                {
                    while (--top)
                        s += (stack[top].y - stack[top - 1].y)*(N - (x + stack[top].high - 1));
                    s += stack[top].y*(N - (x + stack[top].high - 1));
                }
                else
                    if (N != x + mid - 1)
                        tot += N / (N - (x + mid - 1));

                if (sum + tot + f[s] >= M)
                    return;
        }

        backtracking(x, 1, sum, S,b);
    }
    else
        if (a[x][y])
            backtracking(x, y + r[x][y], sum, S, b);
        else
        {
            int i = y;
            while (i - y + 2<N && i<N && i<N - x + y && !a[x][i + 1])
                ++i;
            int j, len = i - y + 1;
            for (i = 0; i<len; ++i)
                for (j = 0; j<len; ++j)
                    a[x + i][y + j] = 1;
            for (; len; --len)
            {
                r[x + len][y] = 0;
                for (i = 0; i<len; ++i)
                    r[x + i][y] = len;
                for (i = 0; i <= len; ++i)
                    d[x + i][y] = len - i;

                struct AS n;
                n.len = len;
                n.x = x;
                n.y = y;
                tmp[sum] = n;

                backtracking(x, y + len, sum + 1, S - len * len, b);
                for (i = 0; i<len; ++i)
                    a[x + len - 1][y + i] = 0;
                for (i = 0; i<len; ++i)
                    a[x + i][y + len - 1] = 0;
            }
        }
}


int mainsolver(int t, int changecolore, QProgressBar *my_progressBar, QLabel *my_label)
{
    progressBar=my_progressBar;
    label=my_label;

    // Установка QProgressBar
    progressBar->setValue(0);

    changecolor = changecolore;

    // Выделение памяти
    memset(f, 127, sizeof(f));
    f[0] = 0;
    for (int i = 1; i <= 2500; ++i)
        {
            for (int j = 1; j*j <= i; ++j)
                f[i] = min(f[i], f[i - j * j]);
            ++f[i];
        }

    M = 20;
    N = 0;
    color = 0;
    index = 1;
    N = t;

    factor = (int)(width / t);

    vector<vector<RGBPIXEL>> b(N*factor, vector<RGBPIXEL>(N*factor));


    // Вызов функции backstrackingа
    memset(a, 0, sizeof(a));
    backtracking(1, 1, 0, N*N, b);



    // Вывод сообщения о всех квадратах и их координатах в QLabel
    label->setText("Минимальное число\nквадратов: " + QString::number(M) + "\n\n");
    for (int i = 0; i < M; ++i)
    {
       building(ans[i].x, ans[i].y, ans[i].len, b);
       label->setText(label->text() + QString::number(i+1) + ". Вершина: (" + QString::number(ans[i].x) + "," + QString::number(ans[i].y) + ") Длина: "+ QString::number(ans[i].len) + "\n\n");
    }
    saveAsPPM(b, 0);
    b.clear();

    progressBar->setValue(100);

    return index;
}
