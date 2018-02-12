#include <iostream>
#include <vector>
#include <QProgressBar>
#include <QLabel>
using namespace std;

typedef struct
{
    unsigned char  rgbBlue;
    unsigned char  rgbGreen;
    unsigned char  rgbRed;
} RGBPIXEL;

typedef struct
{
    int x;
    int y;
    int size;
} squareCoord;


RGBPIXEL setcolor(int n);

void addColorToMatrix(squareCoord, vector<vector<RGBPIXEL>> &a, int);

void building(int x, int y, int size, vector<vector<RGBPIXEL>> &a);

void saveAsPPM(vector<vector<RGBPIXEL>> &b, int i);

inline void backtracking(int x, int y, int sum, int S, vector<vector<RGBPIXEL>> &b);

int mainsolver(int , int, QProgressBar*, QLabel*);
