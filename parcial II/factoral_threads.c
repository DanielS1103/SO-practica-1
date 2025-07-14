#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 4
#define TARGET 20

typedef struct{
    int start;
    int end;
    unsigned long long partial_result;
} ThreadData;

sem_t semaphore;
unsigned long long global_result = 1;

void* calculate_partial(void* arg){
    ThreadData* data = (ThreadData*)arg;
    data->partial_result = 1;

    for(int i = data -> start; i <= data -> end; i++){
        data -> partial_result *= i;
    }

    sem_wait(&semaphore);
    global_result *= data -> partial_result;
    sem_post(&semaphore);

    pthread_exit(NULL);
}

int main(){
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    sem_init(&semaphore, 0, 1);

    int segment = TARGET / NUM_THREADS;
    int start = 1;

    for(int i = 0; i < NUM_THREADS; i++){
        thread_data[i].start = start;
        thread_data[i].end = (i == NUM_THREADS - 1) ? TARGET : start + segment - 1;
        start = thread_data[i].end + 1;

        pthread_create(&threads[i], NULL, calculate_partial, &thread_data[i]);
    }

    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&semaphore);
    printf("El factorial de %d es: %llu\n", TARGET, global_result);

    return 0;
}