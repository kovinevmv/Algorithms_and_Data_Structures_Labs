#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define THREAD_NUM 4

int comp (const void* x, const void* y) {
    return *((int*)x) - *((int*)y);
}

int* generate_array(size_t size) {
    int* array = (int*)malloc(sizeof(int) * size);

    for (int i = 0; i < size; i++) {
        array[i] = -99 + rand() % 199; 
    }

    return array;
}


void merge(int* dest, int* L, int* R, size_t size) {
    int i,j,k;

	// i - to mark the index of left aubarray (L)
	// j - to mark the index of right sub-raay (R)
	// k - to mark the index of merged subarray (dest)
	i = 0; j = 0; k =0;

	while (i < size && j < size) {
		if (L[i] < R[j]) {
            dest[k++] = L[i++];
        }
		else {
            dest[k++] = R[j++];
        }
	}

	while (i < size) {
        dest[k++] = L[i++];
    }
    
    while (j < size) {
        dest[k++] = R[j++];
    }
}

typedef struct {
    int thread_num;
    int size;
    int* arr;
} thread_data_t;

pthread_barrier_t barrier;

void* thread(void* arg) {
    thread_data_t thread_arg = *(thread_data_t*)arg;
    
    int* tmp = NULL;
    if(thread_arg.thread_num < 3) {
        tmp = (int*)malloc(sizeof(int) * 2 * thread_arg.size);
    }

    qsort(thread_arg.arr, thread_arg.size, sizeof(int), comp);

    pthread_barrier_wait(&barrier);

    /*
        Итерация 0 - поток №0 с потоком №1,
        а поток №2 с потоком №3
        остальные ждут
    */
    if (thread_arg.thread_num == 0 || thread_arg.thread_num == 2) {
        merge(tmp, thread_arg.arr, thread_arg.arr + thread_arg.size, thread_arg.size);
        memcpy(thread_arg.arr, tmp, sizeof(int) * 2 * thread_arg.size);
    }

    // Остальные потоки ждут
    pthread_barrier_wait(&barrier);

    /*
        Итерация 1 - поток №1 с потоком №2,
        остальные ждут
    */
    if (thread_arg.thread_num == 1) {
        merge(tmp, thread_arg.arr, thread_arg.arr + thread_arg.size, thread_arg.size);
        memcpy(thread_arg.arr, tmp, sizeof(int) * 2 * thread_arg.size);
    }

    // Остальные потоки ждут
    pthread_barrier_wait(&barrier);

    /*
        Итерация 2 - поток №0 с потоком №1,
        а поток №2 с потоком №3
        остальные ждут
    */
    if (thread_arg.thread_num == 0 || thread_arg.thread_num == 2) {
        merge(tmp, thread_arg.arr, thread_arg.arr + thread_arg.size, thread_arg.size);
        memcpy(thread_arg.arr, tmp, sizeof(int) * 2 * thread_arg.size);
    }

    // Остальные потоки ждут
    pthread_barrier_wait(&barrier);

    /*
        Итерация 3 - поток №1 с потоком №2,
        остальные ждут
    */
    if (thread_arg.thread_num == 1) {
        merge(tmp, thread_arg.arr, thread_arg.arr + thread_arg.size, thread_arg.size);
        memcpy(thread_arg.arr, tmp, sizeof(int) * 2 * thread_arg.size);
    }

    // Остальные потоки ждут
    pthread_barrier_wait(&barrier);

    if (tmp) {
        free(tmp);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    pthread_barrier_init(&barrier, NULL, THREAD_NUM);

    pthread_t ids[THREAD_NUM];
    thread_data_t args[THREAD_NUM];

    int size = atoi(argv[1]);
    int seed = atoi(argv[2]);

    srand(seed);
    int* array = generate_array(size);

    int size_per_thread = size / THREAD_NUM;
    
    for (int i = 0; i < THREAD_NUM; i++) {
        args[i].thread_num = i;
        args[i].size = size_per_thread;
        args[i].arr = array + i * size_per_thread;
        pthread_create(&ids[i], NULL, thread, &args[i]);
    }

    for (int i=0; i < THREAD_NUM; i++) {
        pthread_join(ids[i], NULL);
    }

    free(array);

    return 0;
}