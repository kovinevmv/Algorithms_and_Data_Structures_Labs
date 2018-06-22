#include "stdio.h"
#include "stdlib.h"
void menu(int ch, int arr[], int n){       //Объявление функции Menu
	
	scanf("%d%d", n, ch);
	for (int i = 0; i < n ; i++)
		scanf("%d", arr[i]);
	switch (ch)                        //Оператор Switch (оператор множественного выбора для переменной ch)
	{
	case 0: get_max(arr, n); break; // В случае 0 печатаем результат функции поиска максимального элемента массива
        
	case 1: get_min(arr, n); break; //В случае 1 печатаем результат функции поиска минимального элемента массива 
        
	case 2: get_sum(arr, n); break; // В случае 2 печатаем результат функции подсчета суммы четных элементов массива
        
	case 3: get_count_first_el(arr, n); break; // В случае 3 печатаем результат функции подсчета повтора нулевого элемента массива 
        
	default: printf("Данные некорректны"); //В противном случае выводи сообщение о некорректности данных
	}
}

int get_max(int arr[], int n){ // Объявление функции поиска максимального элемента массива
	int i;
	int max = arr[0];
	for (i = 0;i < n; i++)
	{
		if (arr[i] >= max) max = arr[i];
	}
	printf("%d", max);
}

int get_min(int arr[], int n){ // Объявление функции поиска минимального элемента массива
int i;
	int min = arr[0];
	for (i = 0;i < n; i++)
	{
		if (arr[i] <= min) min = arr[i];
	}
	printf("%d", min);
}

int get_sum(int arr[], int n){ //\\ Объявление функции подсчета суммы четных элементов
int s = 0;
	int i;
	for (i = 0;i < n;i++) {
		if (arr[i] % 2 == 0)
			s = s + arr[i];
	}
	printf("%d", s);
}

int get_count_first_el(int arr[], int n){  // Возвращаем сумму в вызывающую функцию
int i;
int k=0;
	for (i = 0;i < n;i++)
	{
		if (arr[i] == arr[0]) k++;
	}
	printf("%d", k-1);
}

