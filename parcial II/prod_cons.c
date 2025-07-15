// prod_cons.c
// Compilar con: gcc prod_cons.c -o prod_cons -lpthread
//problema del productor consumidor

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_BUFFER 16
#define MAX_DATOS 32

pthread_mutex_t mutex;
pthread_cond_t vacio, lleno;
int buffer[MAX_BUFFER], num_datos = 0;

void *consumidor() {
    int posicion = 0, dato;
    for(int i = 0; i < MAX_DATOS; i++) {
        pthread_mutex_lock(&mutex);
        while(num_datos == 0)
            pthread_cond_wait(&vacio, &mutex);

        dato = buffer[posicion];
        posicion = (posicion + 1) % MAX_BUFFER;
        num_datos--;

        if(num_datos == MAX_BUFFER - 1)
            pthread_cond_signal(&lleno);

        pthread_mutex_unlock(&mutex);
        printf("\nse ha consumido el dato: %d", dato);
        fflush(stdout);
        sleep(1);
    }
    return NULL;
}

void *productor() {
    int posicion = 0;
    for(int i = 0; i < MAX_DATOS; i++) {
        pthread_mutex_lock(&mutex);
        while(num_datos == MAX_BUFFER)
            pthread_cond_wait(&lleno, &mutex);

        buffer[posicion] = i;
        posicion = (posicion + 1) % MAX_BUFFER;
        num_datos++;

        if(num_datos == 1)
            pthread_cond_signal(&vacio);

        pthread_mutex_unlock(&mutex);
        printf("\nse ha producido el dato: %d", i);
        fflush(stdout);
    }
    return NULL;
}

int main() {
    pthread_t idhilo1, idhilo2;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&lleno, NULL);
    pthread_cond_init(&vacio, NULL);

    if (pthread_create(&idhilo1, NULL, productor, NULL) != 0) {
        perror("No puedo crear hilo productor");
        exit(-1);
    }
    if (pthread_create(&idhilo2, NULL, consumidor, NULL) != 0) {
        perror("No puedo crear hilo consumidor");
        exit(-1);
    }

    pthread_join(idhilo2, NULL);
    pthread_join(idhilo1, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&lleno);
    pthread_cond_destroy(&vacio);
    return 0;
}
