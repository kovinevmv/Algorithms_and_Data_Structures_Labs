#include "sys/ipc.h"
#include "sys/shm.h"
#include "sys/sem.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "string.h"
#include "errno.h"


int main(int argc, char* argv[]) {
	
	srand(atoi(argv[4]));
				
    int procCount = atoi(argv[1]);
    int linesCount = atoi(argv[2]); 
    int colsCount = atoi(argv[3]);
    int linesPerProc = linesCount / procCount; 

    int arrayshrid = shmget(IPC_PRIVATE, sizeof(int) * linesCount * colsCount, IPC_CREAT | 0666);
    int mulVectorshrid = shmget(IPC_PRIVATE, sizeof(int) * colsCount, IPC_CREAT | 0666);
    int resultVectorshrid = shmget(IPC_PRIVATE, sizeof(int) * linesCount, IPC_CREAT | 0666);
	
    int* array = (int*)shmat(arrayshrid, NULL, 0);

    int* mulVector = (int*)shmat(mulVectorshrid, NULL, 0);
    int* resultVector = (int*)shmat(resultVectorshrid, NULL, 0);


	for (int i = 0; i < colsCount; i++) {
        mulVector[i] = (-9 + rand() % 19)*1000;
    }
	
    for (int i = 0; i < linesCount; i++) {
        for (int j = 0; j < colsCount; j++) {            
            array[i * colsCount + j] = (-9 + rand() % 19) * 1000; 
        }
    }

    
    for (int i = 0; i < procCount; i++) {    
		pid_t pid = fork();
		if (pid == 0) {
            
            int tmpResultVector = 0; 
			for (int j = i * linesPerProc; j < (i + 1) * linesPerProc; j++) {

                for (int k = 0; k < colsCount; k++) {
                    tmpResultVector += array[i * colsCount + k] * mulVector[k];
                }

                resultVector[j] = tmpResultVector; 
                tmpResultVector = 0;
            }
            exit(0);
        }
        else if (pid > 0) {
            continue; 
        }
        else {
            printf("Error fork");
        }
    }
   
    pid_t pid;
    while (pid = waitpid(-1, NULL, 0)) {
        if (errno == ECHILD) {
            break;
        }
    }

    shmctl(arrayshrid, IPC_RMID, NULL);
    shmctl(mulVectorshrid, IPC_RMID, NULL);
    shmctl(resultVectorshrid, IPC_RMID, NULL);

    return 0;
}
