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
        data -> partial_result = *= i;
    }

    sem_wait(&semaphore);
    global_result *= data -> partial_result;
    sem_post(&semaphore);

    pthread_exit(NULL);
}

int main(){
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    sem_init(&semaphote, 0, 1);

    int segment = TARGET / NUM_THREADS;
    int start = 1;
}