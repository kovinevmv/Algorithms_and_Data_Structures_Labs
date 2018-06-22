// Example of user solution
// Error in 333 line
// Correct: printf("%d\n%d\n%d\n%d\n", dlj + 1, djmax + 1, drjmax - 1, imax);

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct
{
	short		bfType; 
	unsigned long	bfSize;  
	unsigned short	bfReserved1; 
	unsigned short	bfReserved2;
	unsigned long	bfOffBits;

} BITMAPFILEHEADER;


typedef struct
{
	unsigned int    biSize; 
	int             biWidth;
	int             biHeight;  
	unsigned short  biPlanes; 
	unsigned short  biBitCount;
	unsigned int    biCompression;
	unsigned int    biSizeImage;
	int             biXPelsPerMeter;
	int             biYPelsPerMeter;
	unsigned int    biClrUsed; 
	unsigned int    biClrImportant;

} BITMAPINFOHEADER;


typedef struct
{
	unsigned char  rgbBlue;
	unsigned char  rgbGreen;
	unsigned char  rgbRed; 

} RGBPIXEL;



typedef struct Stack
{
	int value;
	struct Stack* next;
} Stack;

void push(Stack** head, int value)
{
	Stack *tmp = (Stack*)malloc(sizeof(struct Stack));
	tmp->value = value;
	tmp->next = *head;
	*head = tmp;
}

void pop(struct Stack **head)
{
	if (*head)
	{
		struct Stack *tmp = *head;
		*head = (*head)->next;
		free(tmp);
	}
}

int top(struct Stack *head)
{
	return head->value;
}

int empty(struct Stack *head)
{
	if (head == NULL)
	{
		return 1;
	}

	else
		return 0;

}


void writeToBitmapHeader(FILE* outputFile, BITMAPFILEHEADER headerBitmap)
{
	fwrite(&headerBitmap.bfType, 2, 1, outputFile);
	fwrite(&headerBitmap.bfSize, 4, 1, outputFile);
	fwrite(&headerBitmap.bfReserved1, 2, 1, outputFile);
	fwrite(&headerBitmap.bfReserved2, 2, 1, outputFile);
	fwrite(&headerBitmap.bfOffBits, 4, 1, outputFile);
}

void writeToBitmapInfo(FILE* outputFile, BITMAPINFOHEADER infoBitmap)
{
	fwrite(&infoBitmap.biSize, 4, 1, outputFile);
	fwrite(&infoBitmap.biWidth, 4, 1, outputFile);
	fwrite(&infoBitmap.biHeight, 4, 1, outputFile);
	fwrite(&infoBitmap.biPlanes, 2, 1, outputFile);
	fwrite(&infoBitmap.biBitCount, 2, 1, outputFile);
	fwrite(&infoBitmap.biCompression, 4, 1, outputFile);
	fwrite(&infoBitmap.biSizeImage, 4, 1, outputFile);
	fwrite(&infoBitmap.biXPelsPerMeter, 4, 1, outputFile);
	fwrite(&infoBitmap.biYPelsPerMeter, 4, 1, outputFile);
	fwrite(&infoBitmap.biClrUsed, 4, 1, outputFile);
	fwrite(&infoBitmap.biClrImportant, 4, 1, outputFile);
}

void writeToBitmapRGB(FILE* outputFile, RGBPIXEL** arrayRGB, BITMAPINFOHEADER infoBitmap)
{

	int extraBytes = 4 - ((infoBitmap.biWidth * 3) % 4);
	if (extraBytes == 4)
	{
		extraBytes = 0;
	}

	for (int i = infoBitmap.biHeight - 1; i >= 0; i--)
	{
		for (int j = 0; j < infoBitmap.biWidth; j++)
		{
			fwrite(&arrayRGB[i][j].rgbBlue, 1, 1, outputFile);
			fwrite(&arrayRGB[i][j].rgbGreen, 1, 1, outputFile);
			fwrite(&arrayRGB[i][j].rgbRed, 1, 1, outputFile);

		}
		if (extraBytes)
		{
			for (int j = 0; j < extraBytes; j++)
			{
				fwrite("0", 1, 1, outputFile);  
			}
		}
	}

}

BITMAPFILEHEADER readFromBitmapHeader(FILE* inputFile)
{
	BITMAPFILEHEADER headerBitmap;
	fread(&headerBitmap.bfType, 2, 1, inputFile);
	fread(&headerBitmap.bfSize, 4, 1, inputFile);
	fread(&headerBitmap.bfReserved1, 2, 1, inputFile);
	fread(&headerBitmap.bfReserved2, 2, 1, inputFile);
	fread(&headerBitmap.bfOffBits, 4, 1, inputFile);
	return headerBitmap;
}

BITMAPINFOHEADER readFromBitmapInfo(FILE* inputFile)
{

	BITMAPINFOHEADER infoBitmap;
	fread(&infoBitmap.biSize, 4, 1, inputFile);
	fread(&infoBitmap.biWidth, 4, 1, inputFile);
	fread(&infoBitmap.biHeight, 4, 1, inputFile);
	fread(&infoBitmap.biPlanes, 2, 1, inputFile);
	fread(&infoBitmap.biBitCount, 2, 1, inputFile);
	fread(&infoBitmap.biCompression, 4, 1, inputFile);
	fread(&infoBitmap.biSizeImage, 4, 1, inputFile);
	fread(&infoBitmap.biXPelsPerMeter, 4, 1, inputFile);
	fread(&infoBitmap.biYPelsPerMeter, 4, 1, inputFile);
	fread(&infoBitmap.biClrUsed, 4, 1, inputFile);
	fread(&infoBitmap.biClrImportant, 4, 1, inputFile);
	return infoBitmap;
}

RGBPIXEL** readFromBitmapRGB(FILE* inputFile, BITMAPINFOHEADER infoBitmap)
{
	RGBPIXEL** arrayRGB = (RGBPIXEL**)malloc(sizeof(RGBPIXEL*)*infoBitmap.biHeight);
	for (int i = 0; i < infoBitmap.biHeight; i++)
	{
		arrayRGB[i] = (RGBPIXEL*)malloc(sizeof(RGBPIXEL)*infoBitmap.biWidth);
	}

	
	int extraBytes = 4 - ((infoBitmap.biWidth * 3) % 4);
	if (extraBytes == 4)
	{
		extraBytes = 0;
	}

	for (int i = infoBitmap.biHeight - 1; i >= 0; i--)
	{
		for (int j = 0; j < infoBitmap.biWidth; j++)
		{
			fread(&arrayRGB[i][j].rgbBlue, 1, 1, inputFile);
			fread(&arrayRGB[i][j].rgbGreen, 1, 1, inputFile);
			fread(&arrayRGB[i][j].rgbRed, 1, 1, inputFile);
		}

		if (extraBytes)
		{
			for (int j = 0; j < extraBytes; j++)
			{
				char zeroByte;
				fread(&zeroByte, 1, 1, inputFile);
			}
		}
	}

	return arrayRGB;

}


void mirrorReflectionBitmap(RGBPIXEL** arrayRGB, BITMAPINFOHEADER infoBitmap)
{
	RGBPIXEL temp;
	for (int i = 0; i < infoBitmap.biHeight; i++)
	{
		int k = infoBitmap.biWidth - 1;
		for (int j = 0; j < k; j++)
		{
			temp = arrayRGB[i][j];
			arrayRGB[i][j] = arrayRGB[i][k];
			arrayRGB[i][k] = temp;
			k--;
		}
	}

}

void drawBlackTriangle(RGBPIXEL** arrayRGB, BITMAPINFOHEADER infoBitmap,
	int x0, int y0, int x1, int y1)
{
	float* steps = (float*)malloc(sizeof(float)*(y0 - y1));
	steps[0] = 0.0;

	for (int i = 1; i < y0 - y1; i++)
	{
		steps[i] = (float)(x1 - x0) / (float)(y0 - y1) + steps[i - 1];
	}

	int k = 0;
	for (int i = (y1 + y0) / 2; i < y0; i++)
	{
		for (int j = x0; j < x0 + (int)steps[k]; j++)
		{
			arrayRGB[i][j].rgbBlue = 0;
			arrayRGB[i][j].rgbRed = 0;
			arrayRGB[i][j].rgbGreen = 0;
		}
		k++;
	}

}

void searchWhiteRectangle(RGBPIXEL** arrayRGB, BITMAPINFOHEADER infoBitmap)
{

	int m = infoBitmap.biWidth;
	int n = infoBitmap.biHeight;
	int imax = 0;
	int djmax = 0;
	int drjmax = 0;
	int dlj = 0;
	int max = 0;

	int* d = (int*)malloc(sizeof(int)*m);
	for (int i = 0; i < m; i++)
	{
		d[i] = -1;
	}
	
	int* dl = (int*)malloc(sizeof(int)*m);
	int* dr = (int*)malloc(sizeof(int)*m);

	Stack* st = NULL;
		

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (!((arrayRGB[i][j].rgbBlue == 255) && 
				(arrayRGB[i][j].rgbRed == 255) && 
				(arrayRGB[i][j].rgbGreen == 255)))
			{
				d[j] = i;
			}
		}

		while (!empty(st))
		{ 
			pop(&st);
		}
			

		for (int j = 0; j < m; j++)
		{
			while (!empty(st) && d[top(st)] <= d[j])
			{
				pop(&st);
			}

			dl[j] = empty(st) ? -1 : top(st);
			push(&st, j);
		}

		while (empty(st))
		{
			pop(&st);
		}

		for (int j = m - 1; j >= 0; --j)
		{
			while (!empty(st) && d[top(st)] <= d[j])
			{
				pop(&st);
			}

			dr[j] = empty(st) ? m : top(st);
			push(&st, j);

		}
		for (int j = 0; j < m; ++j)
		{
			if ((i - d[j]) * (dr[j] - dl[j] - 1) >= max)
			{
				max = (i - d[j]) * (dr[j] - dl[j] - 1);
				imax = i;
				djmax = d[j];
				drjmax = dr[j];
				dlj = dl[j];
			}
		}
	}
	printf("%d\n%d\n%d\n%d\n", dlj + 1, djmax + 1, drjmax - 1, imax + 12314);

}

void writeError(int errorCode)
{
	printf("Fail with ");
	switch (errorCode)
	{
	case 1: printf("input_file"); break;
	case 2: printf("x0"); break;
	case 3: printf("y0"); break;
	case 4: printf("x1"); break;
	case 5: printf("y1"); break;
	case 6: printf("commands"); break;
	}
	printf("\n");
}

int isCorrectCommands(char** data, int lenght, BITMAPINFOHEADER infoBitmap)
{
	if (!((data[0] = "./input_1.bmp") ||
		(data[0] = "./input_2.bmp") ||
		(data[0] = "./input_3.bmp") ||
		(data[0] = "./input_4.bmp") ||
		(data[0] = "./input_5.bmp")))
	{
		writeError(1);
		return 0;
	}
	int x0 = atoi(data[1]);
	int y0 = atoi(data[2]);
	int x1 = atoi(data[3]);
	int y1 = atoi(data[4]);

	if (!(x0 < x1) || (x0 > infoBitmap.biWidth) || (x0 < 0))
	{
		writeError(2);
		return 0;
	}

	if (!(y0 > y1) || (y0 > infoBitmap.biHeight) || (y0 < 0))
	{
		writeError(3);
		return 0;
	}

	if ((x1 > infoBitmap.biWidth) || (x1 < 0))
	{
		writeError(4);
		return 0;
	}

	if ((y1 > infoBitmap.biHeight) || (y1 < 0))
	{
		writeError(5);
		return 0;
	}

	for (int i = 5; i < lenght; i++)
	{
		if ((atoi(data[i]) > 4) || (atoi(data[i]) < 1))
		{
			writeError(6);
			return 0;
		}
	}
	if (atoi(data[lenght - 1]) != 4)
	{
		writeError(6);
		return 0;
	}

}


int main()
{
	char* string = (char*)malloc(sizeof(char) * 500);
	fgets(string, 500, stdin);
	(*strstr(string, "\n")) = 0;

	char** data = (char**)malloc(sizeof(char*) * 25);
	for (int i = 0; i < 25; i++)
	{
		data[i] = NULL;
	}

	int length = 0;
	for (char* c = strtok(string, " "); c; c=strtok(NULL, " "))
	{
		data[length++] = c;
	}

	FILE* inputFile = fopen(data[0], "rb");

	if (!inputFile)
	{
		writeError(1);
		return 1;
	}

	BITMAPFILEHEADER headerBitmap = readFromBitmapHeader(inputFile);
	BITMAPINFOHEADER infoBitmap = readFromBitmapInfo(inputFile);

	RGBPIXEL** arrayRGB = (RGBPIXEL**)malloc(sizeof(RGBPIXEL*)*infoBitmap.biHeight);
	for (int i = 0; i < infoBitmap.biHeight; i++)
	{
		arrayRGB[i] = (RGBPIXEL*)malloc(sizeof(RGBPIXEL)*infoBitmap.biWidth);
	}
	arrayRGB = readFromBitmapRGB(inputFile, infoBitmap);

	fclose(inputFile);

	if (!isCorrectCommands(data, length, infoBitmap))
	{
		return 1;
	}

	
	for (int i = 5; i < length; i++)
	{
		switch (atoi(data[i]))
		{

			case 1: 
			{
				searchWhiteRectangle(arrayRGB, infoBitmap);
				break;
			}
			case 2: 
			{
				drawBlackTriangle(arrayRGB, infoBitmap,
				atoi(data[1]), atoi(data[2]),
				atoi(data[3]), atoi(data[4]));
				break;
			}
			case 3:
			{
				mirrorReflectionBitmap(arrayRGB, infoBitmap);
				break;
			}
			case 4: 
			{
				FILE* outFile = fopen("./usersol.bmp", "wb");

				writeToBitmapHeader(outFile, headerBitmap);
				writeToBitmapInfo(outFile, infoBitmap);
				writeToBitmapRGB(outFile, arrayRGB, infoBitmap);
				fclose(outFile);
			}
		}
	}


	for (int i = 0; i < infoBitmap.biHeight; i++)
	{
		free(arrayRGB[i]);
	}
	free(arrayRGB);
	free(data);
	free(string);

	return 0;
}
