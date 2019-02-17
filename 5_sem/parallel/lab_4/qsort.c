#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

int* generate_array(size_t size) {
    int* array = (int*)malloc(sizeof(int) * size);

    for (int i = 0; i < size; i++) {
        array[i] = -9 + rand() % 19; // [-9, 9]
    }

    return array;
}

void print_array(int* array, size_t size) {
    for (int i = 0; i < size - 1; i++) {
        printf("%d, ", array[i]);
    }
    printf("%d\n", array[size - 1]);
}

int comp (const void* a, const void* b) {
    return *((int*)a) - *((int*)b);
}

int main(int argc, char* argv[]) {
    int size = atoi(argv[1]);
    int seed = atoi(argv[2]);

    srand(seed);
    int* array = generate_array(size);
    
    qsort(array, size, sizeof(int), comp);

    free(array);

    return 0;
}