#include "stdlib.h"
#include "struct.h"
#include "stdio.h"
#include "string.h"
#include "createlist.h"

/* Функция очистки экрана */
void clear()
{
	/* Очистка экрана для Windows и Linux */
	system("@cls||clear");
}

/* Функция задержки экрана */
void pause()
{
	printf("Press any key to continue...\n");
	getchar();
	getchar();
}

/* Функция вывода элементов списка*/
void print_names(MusicalComposition *head)
{
	MusicalComposition *current = head;
        int i = 0;
	/* Пока существует список выполняем цикл*/
        while (current)
        {
		/* Выводим на экран имя, автора и год */
		printf(" %d. %s %s %d\n", i++, current->name, current->author, current->year);
		current = current->next; //Смещаем указатель вперед
        }
}
/* Функция добавления элемента в конец списка*/
void push(MusicalComposition* head)
{
        char name_for_push[80];
        char author_for_push[80];
        int year_for_push;
	/* Считываем имя, автора и год на добавление*/
        printf("Enter name, author and year (using spaces):\n");
        scanf("%s %s %d", name_for_push, author_for_push, &year_for_push);

	/* Выделяем память под новый элемент*/
        MusicalComposition *element = createMusicalComposition(name_for_push, author_for_push, year_for_push);
        MusicalComposition *current = head;
	
	/* Передвигаем указатель в конец списка */
        while (current->next)
                current = current->next;
	
	/* Добавляем элемент в конец списка и устанавливаем связь*/
        current->next = element;
        element->prev = current;
        element->next = NULL;
}

/* Функция добавления нескольких элементов в конец списка */
void pushmanyel(MusicalComposition* head)
{
        int N = 0;
        clear(); //Очищаем экран
	
	/* Считываем количество введенных элементов - N */
        printf("Enter the number of adding elements: ");
        scanf("%d", &N);
        printf("\n");
        int i=0;
	
	/* Вызываем N-раз функцию для добавления элемента через цикл*/
        for (i = 0; i < N; i++)
        {
                push(head);
                printf("\n");
        }
	
	/* Выводим сообщение о выполнении операции */
        printf("Adding completed successfully!\n\n");
        pause();
}

/* Функция подсчета элементов списка */
int count(MusicalComposition*head)
{
        MusicalComposition* current = head;
        int count = 0;
	/* Двигаем указатель пока существет список*/
        while (current)
        {
                count++; //Увеличиваем счетчик - количество элементов
                current = current->next;
        }
        return count;
};

/* Функция удаления элемента */
void removeEl(MusicalComposition* head, char* name_for_remove, int* CreatedList)
{
	/* Двигаем указатель до конца списка */
        while (head)
        {
		/* Если находим в списке находим в списке удовлетворяющую нас позицию, то выполняем следующее:*/
                if (strcmp(head->name, name_for_remove) == 0)
                { 
			/* 1-ый случай. Если элемент - единственный*/
                        if ((head->next == NULL) && (head->prev == NULL))
                        {
				/* Удаляем список*/
                                free(head);
                                head=NULL;
				/* Выводим сообщение об удалении списка*/
                                printf("\nYou removed the last element in the List\nTo continue use this programm create new List!\n");
                                *CreatedList = 0; //Закрываем доступ к опрдеделенным пунктам меню
                                return;
                        }
			
			/*2-ой случай. Если элемент первый и после него идут другие элементы*/
                        else if ((head->prev == NULL) && (head->next))
                        {
				/*Смещаем вперед указатель на следующий элемент*/
                                *head = *head->next;
                                head->prev = NULL;
                                return;
                        }
			
			/* 3-ий случай. Если элемент последний и перед ним идут другие элементы */
                        else if ((head->next == NULL) && (head->prev))
                        {
				/*Смещаем назад указатель на предыдущий элемент*/
                                head = head->prev;
                                head->next = NULL;
                                return;
                        }
			
			/*4-ый случай. Если до и после элемента есть другие элементы*/
                        else if ((head->next) && (head->prev))
                        {
                                head->next->prev = head->prev;
                                head->prev->next = head->next;
                                return;
                        }
                }
                head = head->next;
        }
}

/* Функция сортировки*/
void sort(MusicalComposition *head)
{
        clear(); //Очищаем экран
        printf("Sort\n\n");
        printf("The original List:\n");
        print_names(head); //Выводим исходный список

        MusicalComposition *curr_i = head;
        MusicalComposition *curr_j;

	/* Выполняем сортировку методом пузырька */
        while (curr_i->next)
        {
                curr_j = head;
                while (curr_j->next)
                {
                        if (curr_j->year>curr_j->next->year)
                        {
				/* Обмениваемся значениями между соседними элементами*/
                                MusicalComposition *temp = (MusicalComposition*)malloc(sizeof(MusicalComposition));
                                strcpy(temp->name, curr_j->next->name); strcpy(temp->author, curr_j->next->author); temp->year = curr_j->next->year;
                                strcpy(curr_j->next->name, curr_j->name); strcpy(curr_j->next->author, curr_j->author); curr_j->next->year = curr_j->year;
                                strcpy(curr_j->name, temp->name); strcpy(curr_j->author, temp->author); curr_j->year = temp->year;
                                free(temp);
                        }
                        curr_j = curr_j->next;
                }
                curr_i = curr_i->next;
        }
        printf("\nUpdated List:\n");
        print_names(head); //Выводим измененный список
        printf("\n");
        pause();
}

/*Функция удаления нечетных элементов*/
void removeoddel(MusicalComposition *head)
{
        clear();//Очищаем экран
        printf("Remove all odd elements\n(numbering starts at zero)\n\n");
        printf("The Original List:\n"); 
        print_names(head);//Выводим исходный список
        int i = 0;
        MusicalComposition *current = head;
        while (current)
        {
                if (!((++i) % 2)) //Если номер элемента нечетен, то вызываем функцию удаления
                        removeEl(head, current->name, 0);
                current = current->next;
        }
        printf("\nUpdated List:\n");
        print_names(head); //Выводим исходный список
        printf("\n");
        pause();
}

/* Функция вывода ошибки при отсутствии списка */
void CreatedList()
{
        clear();
        printf("You didn't create List!\nPlease, choose 1 to create List in Menu\n");
        pause();
}
