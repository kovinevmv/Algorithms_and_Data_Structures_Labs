#include "stdio.h"
#include "unistd.h"
#include "errno.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "stdlib.h"

long double calcPartSeries(int start, int end){
	long double result = 0.0;
    for (int i = start; i <= end; i++)
		result += (long double)(((i + 1) % 2 ? -1 : 1) * 4) / (long double)(2 * i - 1);
	return result;
}

int main(int argc, char const *argv[])
{
    int numOfItems = atoi(argv[1]);
    int numOfProcesses = atoi(argv[2]);
    int itemsPerProcess = numOfItems / numOfProcesses;

    int** channels = (int**)malloc(sizeof(int*) * numOfProcesses);
    for (int i = 0; i < numOfProcesses; i++) {
        channels[i] = (int*)malloc(sizeof(int) * 2);
    }

    for (int i = 0; i < numOfProcesses; i++) {
        int res = pipe(channels[i]);

		if (res < 0){
			printf("Error of pipe\n");
			return 1;
		}
        pid_t pid = fork();
        if (pid == 0) {
            close(channels[i][0]);
			long double current_res = calcPartSeries(i * itemsPerProcess + 1, (i + 1) * itemsPerProcess);
            write(channels[i][1],&current_res,sizeof(long double));
            return 0;      
        }
        else if (pid > 0) {
            close(channels[i][1]);
            continue;
        }
        else {
            printf("Error of fork\n");
			return 1;
        }
    }

    pid_t pid;
    while (pid = waitpid(-1, NULL, 0)) {
        if (errno == ECHILD) {
            break;
        }
    }

    long double result = 0.0;
    long double part = 0.0;
    for (int i = 0; i < numOfProcesses; i++) {
        read(channels[i][0], &part, sizeof(long double));
        result += part;
    }

    printf("%.64Lf\n", result);
    
    for (int i = 0; i < numOfProcesses; i++) {
        free(channels[i]);
    }
    free(channels);

    return 0;
}
