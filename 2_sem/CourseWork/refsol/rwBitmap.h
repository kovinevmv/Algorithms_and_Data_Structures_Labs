
/*======================================================================
  Функция WriteToBitmapHeader записывает в файл outputFile 
  структуру данных headerBitmap
*/
void writeToBitmapHeader(FILE* outfile, BITMAPFILEHEADER headerBMP);
//======================================================================


/*======================================================================
  Функция WriteToBitmapInfo записывает в файл outputFile 
  структуру данных infoBitmap
*/
void writeToBitmapInfo(FILE* outfile, BITMAPINFOHEADER infoBMP);
//======================================================================


/*======================================================================
  Функция WriteToBitmapRGB записывает в файл outputFile матрицу пикселей
  arrayRGB, основываясь на размеры изображения из структуры infoBitmap
*/
void writeToBitmapRGB(FILE* outfile, RGBPIXEL** arrayRGB, BITMAPINFOHEADER infoBMP);
//======================================================================


/*======================================================================
  Функция ReadFromBitmapHeader считывает из файла inputFile
  структуру headerBitmap и возвращает ее
*/
BITMAPFILEHEADER readFromBitmapHeader(FILE* bitmap);
//======================================================================



/*======================================================================
  Функция ReadFromBitmapInfo считывает из файла inputFile
  структуру infoBitmap и возвращает ее
*/
BITMAPINFOHEADER readFromBitmapInfo(FILE* bitmap);
//======================================================================



/*======================================================================
  Функция ReadFromBitmapRGB считывает из файла матрицу
  пикселей с учетом размеров изображения и возвращает ее
*/
RGBPIXEL** readFromBitmapRGB(FILE* bitmap, BITMAPINFOHEADER info);
//======================================================================
