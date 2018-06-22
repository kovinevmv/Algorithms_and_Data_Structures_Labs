#include "stdio.h"
#include "stdlib.h"
#include "time.h"

int compare(const void * x1, const void * x2)
{
	return (*(int*)x1 - *(int*)x2);
}

int main()
{
	clock_t time;
	int search_element = 0;
	int check_zero = 0;

	int array_size = 1000;
	int* array;
	array = (int*)malloc(sizeof(int)*array_size);
	for (int i = 0; i < array_size; i++)
		scanf("%d", &array[i]);

	qsort(array, array_size, sizeof(int), compare);

	time = clock();
	int* exist = (int*)bsearch(&search_element, array, array_size, sizeof(int), compare);
	time = clock() - time;

	if (exist != NULL) printf("exists\n");
	else printf("doesn't exist\n");

	printf("%f\n", ((double)time) / CLOCKS_PER_SEC);

	time = clock();
	for (int i = 0; i<array_size; i++)
		if (array[i] == search_element)
		{
			printf("exists\n");
			time = clock() - time;
			check_zero = 1;
			break;
		}
	if (check_zero == 0) printf("doesn't exist\n");
	printf("%f\n", ((double)time) / CLOCKS_PER_SEC);
	
	return 0;
}