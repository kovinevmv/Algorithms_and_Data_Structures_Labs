#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Stack				 // Структура, описывающая стек
{
	int value;			 // Значение
	struct Stack* next;		 // Указатель на следующий элемент стека
};

typedef struct Stack Stack;  // Объявление типа данный Stack

int go = 1;			// Переменная выхода из программы
				// 1 - ошибок не обнаружено, 0 - присутствует ошибка

/* Добавление элемента в стек */
void push(struct Stack **head, int value)
{
	struct Stack *tmp = (Stack*)malloc(sizeof(struct Stack));

	tmp->value = value;		  // Добавление значения в новую структуру
	tmp->next = *head;		  // Осуществление связи
	*head = tmp;

	printf("ok\n");

}

/* Удаление элемента из стека */
void pop(struct Stack **head)
{
	if (*head == NULL) // Проверка на существование стека
	{
		printf("error\n");	 // Стек отсутствует -> удаление невозможно
		go = 0;                  // Ошибка, остановка работы программы

	}
	else
	{
		printf("%d\n", (*head)->value);   // Вывод удаляемого значания на экран
		*head = (*head)->next;		  // Смещение указателя на новое значение головы
	}
	
}

/* Вывод верхнего элемента стека */
void top(struct Stack *head)
{
	if (head == NULL)
	{
		printf("error\n");		// Стек отсутствует -> удаление невозможно
		go = 0;                         // Ошибка, остановка работы программы

	}
	else
	{
		printf("%d\n", head->value);    // Вывод верхнего элемента стека
	}
}

/* Количество элементов в стеке */
void size(Stack* head)
{
	int count = 0;			    // Инициализация счетчика - количество элементов
	while (head != NULL)                // Пока существует стек, увеличиваем счетчик
	{
		count++;
		head = head->next;
	}

	printf("%d\n", count);             // Вывод значения

}

int main()
{
	Stack* head = NULL;
	char* string = (char*)malloc(sizeof(char) * 5);    // Выделение памяти под строку

	while (go == 1)					   // Проверка на наличие ошибок
	{
		fgets(string, 5, stdin);

		if (!(strcmp(string, "push")))
		{
			int value = 0;
			scanf("%d", &value);     // Считывание добавляемого значения
			push(&head, value);	 // Если команда push - выполняем push();
		}
		else if (!(strcmp(string, "pop\n")))
		{
			pop(&head);			// Если команда pop - выполняем pop();
		}
		else if (!(strcmp(string, "top\n")))
		{
			top(head);			// Если команда top - выполняем top();
		}
		else if (!(strcmp(string, "size")))
		{
			size(head);			// Если команда size - выполняем size();
		}

		else if (!(strcmp(string, "exit")))
		{
			printf("bye\n");		// Остановка работы программы 
			return 0;
		}

	}
	free(head);
	return 0;
}
