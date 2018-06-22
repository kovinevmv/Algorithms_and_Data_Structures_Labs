#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "struct.h"    
#include "createlist.h"   
#include "function.h"     

int main()
{
	int decision = 0; //Переменная, отвечающая за выбор пункта меню
	int createdList = 0;  //Переменная, отвечающая за открытие пользователю всех пунктов меню (кроме пунтка создания списка и выхода)
	MusicalComposition *head = NULL;
	while (decision != 9)
	{
		clear();
		printf("\n  ################################ Menu ###############################\n");
		printf("  #                                                                   #\n");
		printf("  #                                                                   #\n");
		printf("  #   1. Create List                                                  #\n");
		printf("  #   2. Print List                                                   #\n");
		printf("  #   3. Sort List by year ascending                                  #\n");
		printf("  #   4. Add new element                                              #\n");
		printf("  #   5. Add several elements                                         #\n");
		printf("  #   6. Remove elements by search appropriate name                   #\n");
		printf("  #   7. Remove all odd elements                                      #\n");
		printf("  #   8. Number of elements                                           #\n");
		printf("  #   9. Exit                                                         #\n");
		printf("  #                                                                   #\n");
		printf("  #                                                                   #\n");
		printf("  #####################################################################\n\n\n");

		printf("  Choose: ");
		scanf("%d", &decision); //Считываем значение, отвечающее за выбор пунка меню

		switch (decision) //Выполняем множественный выбор по переменной decision
		{
		case 1:
			head = add(); //Создаем список, используяя функцию add(), которая возвращает head
			createdList = 1; //Открываем доступ ко всем элементам меню
			break;
		case 2:
			if (createdList != 1) //Проверка на создание списка
				CreatedList();
			else
			{
				clear(); //Очищаем экран от интерфейса меню
				printf("List:\n"); 
				print_names(head); //Используя функцию print_names(), выводим все элементы списка
				printf("\n");
				pause(); //Задерживаем экран консоли 
			}
			break;
		case 3:
			if (createdList != 1) //Проверка на создание списка
				CreatedList();
			else
				sort(head); //Выполняем функцию сортировки
			break;
		case 4:
			if (createdList != 1) //Проверка на создание списка
				CreatedList();
			else
			{
				clear(); //Очищаем экран от интерфейса меню
				push(head); //Выполняем функцтю push() - добавляем новый элемент в конец списка
				printf("\nAdding elements completed successfully!\n\n"); //Выводим сообщение о завершении процесса
				pause(); //Задерживаем экран консоли 
			}
			break;
		case 5:
			if (createdList != 1)//Проверка на создание списка
				CreatedList();
			else
				pushmanyel(head);//Выполняем функцтю pushmanyel() - добавляем несколько новых элементов в конец списка
			break;
		case 6:
			if (createdList != 1)//Проверка на создание списка
				CreatedList();
			else
			{
				clear(); //Очищаем экран от интерфейса меню
				char name_for_remove[80];
				printf("Enter the name to be removed from the List\n");
				scanf("%s", name_for_remove); //Считываем имя, по которому будет произведен  
				removeEl(head, name_for_remove, &createdList); //поиск элемента, необходимого для удаления
				printf("\nRemovig completed successfully!\n\n"); //Выводим сообщение о завершении процесса
				pause(); //Задерживаем экран консоли 
			}
			break;
		case 7:
			if (createdList != 1)//Проверка на создание списка
				CreatedList();
			else
				removeoddel(head);//Выполняем функцтю removeoddel() - удаление всех нечетных элементов списка
			break;
		case 8:
			if (createdList != 1)//Проверка на создание списка
				CreatedList();
			else
			{
				clear(); //Очищаем экран от интерфейса меню
				printf("Number of elements in List: %d\n\n", count(head)); //Выводим количество элементов в списке
				pause();//Задерживаем экран консоли 
			}
			break;
		case 9: break; //Если decision=9, тогда произойдет выход из программы
		default:
			clear(); //Очищаем экран от интерфейса меню
			printf("There is no such option. Try again!\n\n"); //Выводим сообщение о ошибке, что данного пункта в меню нет
			pause();//Задерживаем экран консоли 
		}
	}
	clear();//Очищаем экран
	return 0;
}
