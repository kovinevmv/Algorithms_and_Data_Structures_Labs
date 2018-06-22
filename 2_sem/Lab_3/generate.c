#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

void makeFile(char* name)
{
	FILE* file = fopen(name, "wb");
	int size = rand() % 5 + 1;
	for (int i = 0; i < size; i++)
	{
		fprintf(file, "%d ", rand() % 10 + 1);
	}
	fclose(file);
}


void mk(char* path)
{
	char current_path[10000];

	strcpy(current_path, path);

	char fileNameIndex[3];
	int r = rand() % 2 + 2;
	for (int i = 1; i <= r; i++)
	{
		sprintf(fileNameIndex, "%d", i);
		char tt[10000];
		strcpy(tt, current_path);
		strcat(tt, "/file");
		strcat(tt, fileNameIndex);
		strcat(tt, ".txt");
		printf("%s\n", tt);
		makeFile(tt);
	}

	r = rand() % 2;
	for (int i = 0; i < r; i++)
	{
		char tt[10000];
		strcpy(tt, current_path);
		if (rand() % 2)
		{
			strcat(tt, "/add");
		}
		else
		{
			strcat(tt, "/mul");
		}
		mkdir(tt, 0777);
	}


	DIR *dir = opendir(current_path);
	struct dirent *de = readdir(dir);
	if (dir)
	{
		while (de)
		{
			int path_len = strlen(current_path);
			strcat(current_path, "/");
			strcat(current_path, de->d_name);

			if (de->d_type == DT_DIR && strcmp(".", de->d_name) && strcmp("..", de->d_name))
			{
				mk(current_path);
			}

			current_path[path_len] = '\0';
			de = readdir(dir);
		}
	}
	closedir(dir);


}
int main()
{
	srand(time(NULL));
	system("cls||clear");
	printf("Tree\n\n");

	if (rand() % 2)
	{
		mkdir("./add", 0777);
		mk("./add");
	}
	else
	{
		mkdir("./mul", 0777);
		mk("./mul");

	}

	printf("\nEnd\n\n");
	return 0;
}

