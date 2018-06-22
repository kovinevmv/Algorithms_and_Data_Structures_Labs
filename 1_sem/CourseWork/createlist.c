#include "struct.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "function.h"

/* Функция создания и инициализации нового экземпляра структуры "MusicalComposition" */
MusicalComposition* createMusicalComposition(char *name, char* author, int year)
{
        /* Выделим память для нового экземпляра структуры "MusicalComposition" */
        MusicalComposition* rr = (MusicalComposition*)malloc(sizeof(MusicalComposition));
        
        /* Проинициализируем поля параметрами, переданными в функцию */
        strcpy(rr->name, name);
        strcpy(rr->author, author);
        rr->year = year;
        rr->next = NULL;
        rr->prev = NULL;
        
        /* Вернем указатель на новый экземпляр структуры "MusicalComposition" */
        return rr;
}

/* Функция создания списка  */
MusicalComposition* createMusicalCompositionList(char** array_names, char** array_authors, int* array_years, int n)
{
        /* Указатель на первый элемент, который вернет функция */
        MusicalComposition *head = createMusicalComposition(array_names[0], array_authors[0], array_years[0]);
        
        /* Указательи для движения по списку  */
        MusicalComposition *prev = head;
        MusicalComposition *current;
        int i=1;
        /* Заполняем список данными, переданными массивами array_names, array_authors, array_years */
        for (i = 1; i < n; ++i)
        {
                current = createMusicalComposition(array_names[i], array_authors[i], array_years[i]);
		
		/* Двигаем указатели */
                current->prev = prev;
                prev->next = current;
                prev = current;
        }
	/* Возвращаем указатель на первый элемент списка - head */
        return head;
}

/* Функция считывания значений списка*/
MusicalComposition* add() 
{	
        clear(); //Очищаем экран
        int length = 0;
	
	/* Выполняем цикл пока не найдем подходящее значение количества элементов - целое неотрицательное число */
        while (length <= 0) 
        {
		/* Считываем значение количества элементов */
                printf("Enter the number of adding elements: ");
                scanf("%d", &length);
		/* Если считанное значение не удовлетворяет условию, выводим сообщение об ошибке */
                if (length <= 0)
                        printf("\nError - number of elements is a positive integer!\n\n");
        }
	
	/* Выделяем память массивы */
        char **names = (char**)malloc(sizeof(char*)*length);
        char **authors = (char**)malloc(sizeof(char*)*length);
        int *years = (int*)malloc(sizeof(int)*length);

        int i=0;
	
	/* Через цикл считываем элементы списка (имя, автор и год) и сохраняем в массивы */
        for (i = 0; i < length; ++i)
        {
                printf("\nEnter element %d: - name, author, year (using spaces):\n", i + 1);
                char name[80];
                char author[80];

                scanf("%s %s %d", name, author, &years[i]);

                names[i] = (char*)malloc(sizeof(char*)*(strlen(name) + 1));
                authors[i] = (char*)malloc(sizeof(char*)*(strlen(author) + 1));

                strcpy(names[i], name);
                strcpy(authors[i], author);
        }
        return createMusicalCompositionList(names, authors, years, length);
}

