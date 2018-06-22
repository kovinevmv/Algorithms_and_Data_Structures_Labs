/*
 * Файл lab3_progin_c_2sem.c
 * Обход дерева папок
 * Автор: kovinevmv
 * 21.05.2017
*/

#include <stdio.h>
#include <dirent.h>
#include <string.h>

/*=============================================================
  Функция readFile получает в качестве аргументов путь к файлу
  current_path и имя папки folderName (add или mul), в которой 
  находится файл. Функция выполняет определенные арифметические 
  операции с числами из файла в зависимости от имени папки и 
  возвращает результат.
*/
long long int readFile(char* current_path, char* folderName);
//=============================================================


/*=============================================================
  Функция searchInDir получает в качестве аргументов текущий 
  путь и папку, в которой находится текущий каталог (add или mul)
  Если в директории есть другая директория, тогда рекурсивно 
  вызывается функция searchInDir, которая добавляет или умножает
  текущий результат на возвращаемое значение. Аналогичные действия 
  выполняются если в директории есть файл TXT, только вызывается
  функция readFile.
*/
long long int searchInDir(const char* path, char* prevFolderName);
//=============================================================



int main()
{
	printf("\nTotal result: %lld\n\n", searchInDir(".", ""));
	return 0;
}



long long int readFile(char* current_path, char* folderName)
{

	long long int result = 0;   // Результат выполнения операций

	int mathOperation;     // Математическая операция, которая будет выполнена 
						   // с числами из файла: 0 - сложение, 1 - умножение

	// Распознавание математической операции
	// mul - умножение, add - сложение
	if (!strcmp(folderName, "mul"))
	{
		result = 1;
		mathOperation = 1;
	}
	else if (!strcmp(folderName, "add"))
	{
		result = 0;
		mathOperation = 0;
	}

	// Открытие файла
	FILE* fileTXT = fopen(current_path, "r");   

	// Считываем каждое число пока файл не закончится 
	while (!feof(fileTXT))
	{
		int value;
		fscanf(fileTXT, "%d ", &value);

		// Выполнение соответствующей операции
		if (mathOperation)
		{
			result *= value;
		}
		else
		{
			result += value;
		}

	}

	// Вывод результата выполнения действий в файле, имеющего путь current_path
	printf("Result: %lld\t File: %s\n", result, current_path);

	fclose(fileTXT);
	return result;

}

long long int searchInDir(const char* path, char* prevFolderName)
{
	long long int result = 0;
	int mathOperation = 0;

	// Распознавание математической операции
	// mul - умножение, add - сложение
	if (!strcmp(prevFolderName, "mul"))
	{
		result = 1;
		mathOperation = 1;
	}
	else if (!strcmp(prevFolderName, "add"))
	{
		result = 0;
		mathOperation = 0;
	}

	char current_path[10000];
	strcpy(current_path, path);

	// Открываем текущую директорию
	DIR *dir = opendir(current_path);
	struct  dirent *de = readdir(dir);


	if (dir)
	{
		while (de)
		{

			// current_path - полный путь к текущему файлу открытой директории
			int path_len = strlen(current_path);
			strcat(current_path, "/");
			strcat(current_path, de->d_name);

			// Если это не текущий и не родительский каталог, 
			// то рекурсивный вызов функции обхода searchInDir
			if (de->d_type == DT_DIR && strcmp(".", de->d_name) && strcmp("..", de->d_name))
			{

				if (mathOperation)
				{
					result *= searchInDir(current_path, de->d_name);
				}
				else
				{
					result += searchInDir(current_path, de->d_name);
				}

			}

			// Если это файл, оканчивающийся .txt, то
			// вызов функции readFile 
			else if (strstr(de->d_name, ".txt") && (de->d_type == 8))
			{
				if (mathOperation)
				{
					result *= readFile(current_path, prevFolderName);
				}
				else
				{
					result += readFile(current_path, prevFolderName);
				}

			}

			// Добавление символа конца строки
			current_path[path_len] = '\0';

			// Считывание нового элемента директории
			de = readdir(dir);
		}
	}
	closedir(dir);
	return result;

}
