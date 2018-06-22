/*
* File generate.c
* Generate 5 Bitmap Images and Numeric Data Text Document
* Written by kovinevmv
* 26.05.2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "struct.h"
#include "rwBitmap.h"


#define WIDTH      640        // Ширина изображения
#define HEIGHT     360        // Высота изображения
#define TYPE       0x4D42     // Тип файла (должен быть BM)
#define SIZEINFO   40         // Размер структуры BITMAPINFOHEADER
#define SIZEHEADER 14         // Размер структуры BITMAPFILEHEADER
#define BITCOUNT   24         // Число битов на пиксель (24 бита)


/*======================================================================

  Функция addColorToBitmap заполняет переданный массив структур,
  отвечающий за пиксели изображения, произвольным образом данными
  одним из трех вариантов: белым цветом, переданным цветом фона 
  или абсолютно произвольным RGB цветом 
*/
void addColorToBitmap(RGBPIXEL** arrayRGB, BITMAPINFOHEADER infoBMP,
	int desiredColorRed, int desiredColorGreen, int desiredColorBlue);
//======================================================================



/*======================================================================

  Функция addRectangleToBitmap заполняет переданный массив структур,
  отвечающий за пиксели изображения, произвольными прямоугольниками.
*/
void addRectangleToBitmap(RGBPIXEL** arrayRGB, BITMAPINFOHEADER infoBMP,
	int numberOfRectangles, int desiredColorRed, int desiredColorGreen,
	int desiredColorBlue);
//======================================================================



/*======================================================================

  Функция createBitmapFile создает BMP изображение размерами
  WIDTH * HEIGHT, с номером fileNameIndex и цветом фона desiredColorRed,
  desiredColorGreen, desiredColorBlue, переданными данной функции
*/
void createBitmapFile(char* fileNameIndex, int desiredColorRed,
	int desiredColorGreen, int desiredColorBlue);
//======================================================================



/*======================================================================
  Функция createTxtFile создает файл input.txt с командами.
  Количество команд - 50.
*/
void createTxtFile();
//======================================================================

int main()
{
	srand(time(NULL));
	
	unsigned int desiredColorRed, desiredColorGreen, desiredColorBlue;

	char fileNameIndex[3];  // Номер изображения

	for (int i = 1; i <= 5; i++)
	{
		// Преобразование номера изображения в строку
		sprintf(fileNameIndex, "%d", i);

		desiredColorRed = rand() % 255;     //
		desiredColorGreen = rand() % 255;   // Генерация цвета фона
		desiredColorBlue = rand() % 255;    //

		// Создание изображения
		createBitmapFile(fileNameIndex, desiredColorRed, desiredColorGreen, 
			desiredColorBlue);
	}

	// Создание файла с командами
	createTxtFile();

	return 0;
}


void createBitmapFile(char* fileNameIndex, int desiredColorRed, 
	int desiredColorGreen, int desiredColorBlue)
{
	char fileName[30] = "./input_";   // Создание имени изображения 
	strcat(fileName, fileNameIndex);  // с учетом его номера
	strcat(fileName, ".bmp");         //

	FILE* outfileBMP = fopen(fileName, "wb");

	BITMAPFILEHEADER headerBMP;
	BITMAPINFOHEADER infoBMP;

	// Количество байтов необходимых для выравнивания изображения
	int extraBytes = 4 - ((WIDTH * 3) % 4);
	if (extraBytes == 4)
	{
		extraBytes = 0;
	}

	// Полный размер изображения с учетом байтов для выравнивания
	int paddedSize = ((WIDTH * 3) + extraBytes) * HEIGHT;

	// Заполнение структур BMP изображения
	headerBMP.bfType = TYPE;
	headerBMP.bfSize = SIZEINFO + SIZEHEADER + paddedSize;
	headerBMP.bfReserved1 = 0;
	headerBMP.bfReserved2 = 0;
	headerBMP.bfOffBits = SIZEINFO + SIZEHEADER;

	infoBMP.biSize = SIZEINFO;
	infoBMP.biWidth = WIDTH;
	infoBMP.biHeight = HEIGHT;
	infoBMP.biPlanes = 1;
	infoBMP.biBitCount = BITCOUNT;
	infoBMP.biCompression = 0;
	infoBMP.biSizeImage = paddedSize;
	infoBMP.biXPelsPerMeter = 0;
	infoBMP.biYPelsPerMeter = 0;
	infoBMP.biClrUsed = 0;
	infoBMP.biClrImportant = 0;

	// Инициализация двумерного массива структур
	RGBPIXEL** arrayRGB = (RGBPIXEL**)malloc(sizeof(RGBPIXEL*)*HEIGHT);
	for (int i = 0; i < HEIGHT; i++)
	{
		arrayRGB[i] = (RGBPIXEL*)malloc(sizeof(RGBPIXEL)*WIDTH);
	}
	
	// Вызов функции addColorToBitmap
	addColorToBitmap(arrayRGB, infoBMP, desiredColorRed,
		desiredColorGreen, desiredColorBlue);

	// Вызов функций addRectangleToBitmap
	addRectangleToBitmap(arrayRGB, infoBMP, 15, 255, 255, 255);
	addRectangleToBitmap(arrayRGB, infoBMP, 15, desiredColorRed,
		desiredColorGreen, desiredColorBlue);

	// Запись данных BMP изображения
	writeToBitmapHeader(outfileBMP, headerBMP);
	writeToBitmapInfo(outfileBMP, infoBMP);
	writeToBitmapRGB(outfileBMP, arrayRGB, infoBMP);

	// Освобождение памяти
	for (int i = 0; i < HEIGHT; i++)
	{
		free(arrayRGB[i]);
	}

	free(arrayRGB);

	fclose(outfileBMP);
}

void addColorToBitmap(RGBPIXEL** arrayRGB, BITMAPINFOHEADER infoBMP,
	int desiredColorRed, int desiredColorGreen, int desiredColorBlue)
{

	for (int i = 0; i < infoBMP.biHeight; i++)
	{
		for (int j = 0; j < infoBMP.biWidth; j++)
		{

			// Переменная, отвечающая за цвет пикселя
			// 0 - белый, 1 - рандомный, 2 - переданный
			int colorFrequency;      
			colorFrequency = rand() % 3; 

			if (colorFrequency == 0)
			{
				arrayRGB[i][j].rgbBlue = 255;   // Заполнение
				arrayRGB[i][j].rgbGreen = 255;  // пикселя белым 
				arrayRGB[i][j].rgbRed = 255;    // цветом
			}

			else if (colorFrequency == 1)
			{
				arrayRGB[i][j].rgbBlue = rand() % 256;    // Заполнение 
				arrayRGB[i][j].rgbGreen = rand() % 256;   // пикселя рандомным
				arrayRGB[i][j].rgbRed = rand() % 256;     // цветом
			}

			else
			{
				arrayRGB[i][j].rgbBlue = desiredColorBlue;    // Заполнение 
				arrayRGB[i][j].rgbGreen = desiredColorGreen;  // пикселя переданным
				arrayRGB[i][j].rgbRed = desiredColorRed;      // цветом
			}

		}
	}

}

void addRectangleToBitmap(RGBPIXEL** arrayRGB, BITMAPINFOHEADER infoBMP,
	int numberOfRectangles, int desiredColorRed, int desiredColorGreen,
	int desiredColorBlue)
{

	for (int k = 0; k < numberOfRectangles; k++)
	{
		// Инициализация размеров добавляемых прямоуголников
		int RectangleX0Coordinates = rand() % (infoBMP.biWidth * 13 / 15 - 1);
		int RectangleY0Coordinates = rand() % (infoBMP.biHeight * 13 / 15 - 1);
		int RectangleWidth = rand() % (infoBMP.biWidth / 10) + 1;
		int RectangleHeight = rand() % (infoBMP.biHeight / 10) + 1;

		for (int i = RectangleY0Coordinates;
			i < RectangleY0Coordinates + RectangleHeight; i++)
		{
			for (int j = RectangleX0Coordinates;
				j < RectangleX0Coordinates + RectangleWidth; j++)
			{
				arrayRGB[i][j].rgbBlue = desiredColorBlue;    // Заполнение 
				arrayRGB[i][j].rgbGreen = desiredColorGreen;  // пикселя переданным
				arrayRGB[i][j].rgbRed = desiredColorRed;      // цветом
			}
		}
	}

}

void createTxtFile()
{
	FILE* outfileTXT = fopen("./input.txt", "w");

	// Запись правильных команд первого теста
	fprintf(outfileTXT, "./input_1.bmp 10 50 20 30 1 2 3 3 2 1 4\n");

	for (int i = 1; i < 50; i++)
	{
		// Переменная, отвечающая за количество команд
		int commandsSize = rand() % 12 + 1;


		// Запись пути BMP изображения
		fprintf(outfileTXT, "%s", "./input_");

		// Переменная isCorrectCommand отвечает за 
		// корректность генерируемых команд
		// 0 - некорректные, 1 - корректные
		int isCorrectCommand = rand() % 2;


		if (isCorrectCommand)
		{
			// Запись правильного индекса изображения
			fprintf(outfileTXT, "%d.bmp ", rand() % 5 + 1);
		}
		else
		{
			// Запись неправильного индекса изображения
			fprintf(outfileTXT, "%d.bmp ", rand() % 10 + 1);
		}


		isCorrectCommand = rand() % 2;
		if (!isCorrectCommand)
		{
			// Запись неправильных координат области изображения
			fprintf(outfileTXT, "%d %d ", rand() % 5000 - 1000, rand() % 5000 - 1000);
			fprintf(outfileTXT, "%d %d ", rand() % 5000 - 1000, rand() % 5000 - 1000);
		}
		else
		{
			// Генерация корректных команд

			int x0 = 1;
			int y0 = 0;
			int x1 = 0;
			int y1 = 1;

			// Генерация коррекнтых координат по Х
			// Х0 должен быть больше, чем Х1
			while (x0 >= x1)
			{
				x0 = rand() % WIDTH;
				x1 = rand() % HEIGHT;
			}

			// Генерация коррекнтых координат по Y
			// Y1 должен быть больше, чем Y0
			while (y1 >= y0)
			{
				y0 = rand() % WIDTH;
				y1 = rand() % HEIGHT;
			}

			// Запись правильных координат области изображения
			fprintf(outfileTXT, "%d %d ", x0, y0);
			fprintf(outfileTXT, "%d %d ", x1, y1);
		}


		isCorrectCommand = rand() % 2;
		if (!isCorrectCommand)
		{
			// Запись неправильных команд
			for (int i = 0; i < commandsSize; i++)
			{
				fprintf(outfileTXT, "%d ", rand() % 7);
			}

			fprintf(outfileTXT, "\n");
		}
		else
		{
			// Запись правильных команд
			for (int i = 0; i < commandsSize; i++)
			{
				fprintf(outfileTXT, "%d ", rand() % 3 + 1);
			}

			fprintf(outfileTXT, "%d\n", 4);
		}
	}

	fclose(outfileTXT);
}
