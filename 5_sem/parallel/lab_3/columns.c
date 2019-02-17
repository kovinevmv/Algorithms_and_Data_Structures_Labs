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

int* calcPart(int start, int end, int linesCount, int colsCount, 
				int* mulVector, int* array){
	int* resultVector_ = (int*)malloc(sizeof(int) * linesCount);
	for (int k = start; k < end; k++) {
		for (int j = 0; j < linesCount; j++) {
			resultVector_[j] += array[j * colsCount + k] * mulVector[k];
        }
    }
	return resultVector_;
}

int main(int argc, char* argv[]) {
	srand(atoi(argv[4]));
	
    int procCount = atoi(argv[1]); 
    int linesCount = atoi(argv[2]);
    int colsCount = atoi(argv[3]);
    int linesPerProc = colsCount / procCount;

    int arrayshrid = shmget(IPC_PRIVATE, sizeof(int) * linesCount * colsCount, IPC_CREAT | 0666);
    int mulVectorshrid = shmget(IPC_PRIVATE, sizeof(int) * colsCount, IPC_CREAT | 0666);
    int resultVectorshrid = shmget(IPC_PRIVATE, sizeof(int) * linesCount, IPC_CREAT | 0666);

    int* array = (int*)shmat(arrayshrid, NULL, 0);
    int* mulVector = (int*)shmat(mulVectorshrid, NULL, 0);
    int* resultVector = (int*)shmat(resultVectorshrid, NULL, 0);
	memset(resultVector, 0, sizeof(int) * linesCount);
	
	int semaforId = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);

    struct sembuf init;
    init.sem_num = 0;
    init.sem_op = 1;
    init.sem_flg = 0;
    semop(semaforId, &init, 1);


	for (int i = 0; i < colsCount; i++) {
        mulVector[i] = (-9 + rand() % 19) *1000;
    }
    
    for (int i = 0; i < linesCount; i++) {
        for (int j = 0; j < colsCount; j++) {
            array[i * colsCount + j] = (-9 + rand() % 19) *1000;
        }
    }

    for (int i = 0; i < procCount; i++) {
        pid_t pid = fork();
        if (pid == 0) {
         
            int* resultVector_ = (int*)malloc(sizeof(int) * linesCount); 
            memset(resultVector_, 0, sizeof(int) * linesCount);
           
            resultVector_ = calcPart(i * linesPerProc, (i + 1) * linesPerProc,
									 linesCount, colsCount, mulVector, array);
            
            struct sembuf lock;
            lock.sem_num = 0;
            lock.sem_op = -1;
            lock.sem_flg = 0;
            semop(semaforId, &lock, 1);

            for (int j = 0; j < linesCount; j++) {
                resultVector[j] += resultVector_[j];
            }
            
            lock.sem_num = 0;
            lock.sem_op = 1;
            lock.sem_flg = 0;
            semop(semaforId, &lock, 1);

            free(resultVector_);

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
    semctl(semaforId, 0, IPC_RMID, NULL);

    return 0;
}