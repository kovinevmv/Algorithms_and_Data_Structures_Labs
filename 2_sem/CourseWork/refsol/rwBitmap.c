#include <stdio.h>
#include <stdlib.h>
#include "struct.h"

void writeToBitmapHeader(FILE* outputFile, BITMAPFILEHEADER headerBitmap)
{
	fwrite(&headerBitmap, sizeof(BITMAPFILEHEADER), 1, outputFile);
}

void writeToBitmapInfo(FILE* outputFile, BITMAPINFOHEADER infoBitmap)
{
	fwrite(&infoBitmap, sizeof(BITMAPINFOHEADER), 1, outputFile);
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
	fread(&headerBitmap, sizeof(BITMAPFILEHEADER), 1, inputFile);
	return headerBitmap;
}

BITMAPINFOHEADER readFromBitmapInfo(FILE* inputFile)
{
	BITMAPINFOHEADER infoBitmap;
	fread(&infoBitmap, sizeof(BITMAPINFOHEADER), 1, inputFile);
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
