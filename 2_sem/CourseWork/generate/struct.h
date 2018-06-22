#pragma pack(push)
#pragma pack(2) 

/*======================================================================
  Структура BITMAPFILEHEADER, содержащая
  информацию о типе, размере и макете файла
*/
typedef struct
{
	short		bfType;       // Тип файла (должен быть BM)
	unsigned int	bfSize;       // Размер файла
	unsigned short	bfReserved1;  // Зарезервирован (должен быть 0)
	unsigned short	bfReserved2;  // Зарезервирован (должен быть 0)
	unsigned int	bfOffBits;    // Смещение от начала битов

} BITMAPFILEHEADER;
//======================================================================



/*======================================================================
  Структура BITMAPINFOHEADER, содержащая
  информацию о размерах и цветовом формате
*/
typedef struct
{
	unsigned int    biSize;          // Число байтов необходимое для структуры
	int             biWidth;         // Ширина изображения (в пикселях)
	int             biHeight;        // Высота изображения (в пикселях)
	unsigned short  biPlanes;        // Число плоскостей (должен быть 1)
	unsigned short  biBitCount;      // Число битов на пиксель (24 бита)
	unsigned int    biCompression;   // Тип сжатия
	unsigned int    biSizeImage;     // Размер изображения (в байтах)
	int             biXPelsPerMeter; // Разрешающая способность по горизонтали
	int             biYPelsPerMeter; // Разрешающая способность по вертикали
	unsigned int    biClrUsed;       // Количество используемых цветов
	unsigned int    biClrImportant;  // Количество существенных цветов

} BITMAPINFOHEADER;
//======================================================================



/*======================================================================
  Структура RGBPIXEL, содержащая информацию о пикселе изображения
*/
typedef struct
{
	unsigned char  rgbBlue;   // Синий   цвет (от 0 до 255)
	unsigned char  rgbGreen;  // Зеленый цвет (от 0 до 255)
	unsigned char  rgbRed;    // Красный цвет (от 0 до 255)

} RGBPIXEL;
//======================================================================


#pragma pack(pop)
