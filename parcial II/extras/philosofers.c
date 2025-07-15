#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_FILOSOFOS 5

pthread_t filosofos[NUM_FILOSOFOS];
sem_t tenedores[NUM_FILOSOFOS];
int ejecutar = 1;

void *filosofo(void *arg){
    int id = *((int *)arg);
    int izquierda = id;
    int derecha = (id + 1) % NUM_FILOSOFOS;

    while(ejecutar){
        printf("Filosofo %d piensa\n", id);
        usleep(rand() % 1000000); //simula el pensamiento con una pausa aleatoria

        printf("Filosofo %d hambriento\n", id);

        sem_wait(&tenedores[izquierda]); // espera por el tenedor izquierdo
        sem_wait(&tenedores[derecha]); // espera por el tenedor derecho

        printf("Filosofo %d usa los cubiertos (%d, %d)\n", id, izquierda, derecha);

        printf("Filosofo %d come\n", id);
        usleep(rand() % 1000000); //simula comer con una pausa aleatoria

        sem_post(&tenedores[izquierda]); //devuelve el tenedor izquierdo
        sem_post(&tenedores[derecha]); //devuelve el tenedor derecho

        printf("Filosofo %d regresa los cubiertos (%d, %d)\n");
    }
    pthread_exit(NULL);
}

int main(){
    srand(time(NULL));
    int i;
    int id[NUM_FILOSOFOS];

    for(i = 0; i < NUM_FILOSOFOS; i++){
        sem_init(&tenedores[i], 0 ,1);
        id[i] = i;
        pthread_create(&filosofos[i], NULL, filosofo, &id[i]);
    }

    sleep(10);

    ejecutar = 0;

    for(i = 0; i < NUM_FILOSOFOS; i++){
        pthread_join(filosofos[i], NULL);
        sem_destroy(&tenedores[i]);
    }

    return 0;
}