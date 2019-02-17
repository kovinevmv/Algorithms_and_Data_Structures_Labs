#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/wait.h"

void printHead(){
	printf("|№\t|Name\t|PID\t|PPID\t|\n");
	printf("| --- | --- | --- | --- |\n");
}

void printProc(int proc_id){
	printf("|%d\t|Process %d\t|%d\t|%d\t|\n", proc_id, proc_id, getpid(), getppid());
}
void printError(int id){
	printf("Error to create process %d\n", id);
}

int main(){
    printHead();
    printProc(1);

    pid_t pid = fork();
    if (pid == 0) {
        // P2 from P1
        printProc(2);

        pid = fork();
        if (pid == 0) {
            // P3 from P2
            printProc(3);
			
			pid = fork();
            if (pid == 0) {
                // P4 from P3
                printProc(4);
            }
            else if (pid > 0) {
                // End P3
                wait(NULL);    
            }
            else {
                printError(4);   
            }
        }
        else if (pid > 0) {
            // End P2  
			wait(NULL);
        }
        else {
			printError(2);
		}
    }
    else if (pid > 0) {
        // End P1
        wait(NULL);
    }
    else {
		printError(2);
    }
}
