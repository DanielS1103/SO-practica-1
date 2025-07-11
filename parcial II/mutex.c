#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 5

int contador = 0;
pthread_mutex_t mutex;

void *funcion_hilos(void *arg){
    int i;
    for(i = 0; i < 100000; i++){
        pthread_mutex_lock(&mutex);
        contador++;
        pthread_mutex_unlock(&mutex);
    }
}

int main(){
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&mutex,NULL);

    int i;

    for(i=0; i < NUM_THREADS; i++){
        pthread_create(&threads[i], NULL, funcion_hilos, NULL);
    }
    for(i=0; i < NUM_THREADS; i++){
        pthread_join(threads[i],NULL);
    }
    printf("el valor final del contador es: %d\n", contador);
    pthread_mutex_destroy(&mutex);
    return 0;
}