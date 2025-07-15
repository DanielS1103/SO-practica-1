#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>


#define NUM_CHAIRS 4  //sillas en la sala de espera
#define NUM_CUSTOMERS 20  //clientes totales a simular

int waiting;  //clientes sentados esperando

sem_t sem_customers; //cuenta clientes que esperan
sem_t sem_barber; //0 = silla del barbero ocupada, 1 = libre

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//RUTINA DEL BARBERO

void *barbero(void *arg){
    (void)arg;
    while(1){
        //dormir hasta que llege alguien
        sem_wait(&sem_customers);

        //tomar un cliente de la sala de espera
        pthread_mutex_lock(&mutex);
        waiting--;
        printf("El barbero invita a pasar, esperando: %d\n", waiting);

        //avisar al cliente que la silla quedo libre
        sem_post(&sem_barber); //el cliente se sienta

        //cortar el cabello
        printf("barbero cortando el cabello");
        sleep(rand() % 3 + 1); //1-3 seg
        printf("el barbero termino\n");
    }
    return NULL;
}

void *cliente(void *arg){
    int id = *(int *)arg;
    free(arg); //el id venia de un malloc
    printf("cliente %d llega\n", id);

    pthread_mutex_lock(&mutex);
    if(waiting < NUM_CHAIRS){
        waiting++;
        printf("cliente %d se sienta, esperando: %d", waiting, id);

        //notificar al barbero que hay alguien
        sem_post(&sem_customers);
        pthread_mutex_unlock(&mutex);

        //esperar a que el barbero libere la silla
        sem_wait(&sem_barber);
        printf("cliente %d recibe un corte\n", id);

        //corte terminado: el hilo puede terminar
        printf("el cliente %d sale feliz\n", id);
    }
    else{
        pthread_mutex_unlock(&mutex);
        printf("el cliente %d se va (sin sillas libres)\n", id);
    }
    return NULL;
}

int main(){
    srand((unsigned)time(NULL));
    //inicializar semaforos
    sem_init(&sem_customers, 0, 0); //0 clientes esperando al inicio
    sem_init(&sem_barber, 0, 0); //barbero dormido, silla libre

    pthread_t barber_thread;
    pthread_create(&barber_thread, NULL, barbero, NULL);

    //crear clientes con llegadas espaciadas
    for(int i = 1; i <= NUM_CUSTOMERS; ++i){
        pthread_t cust_thread;
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&cust_thread, NULL, cliente, id);

        //pequeño retardo para simular tiempos de llegada distintos
        usleep((rand() % 3000 + 1000) * 1000);
    }

    //esperar a que todos los clientes terminen
    pthread_join(barber_thread, NULL); //infinito

    sem_destroy(&sem_customers);
    sem_destroy(&sem_barber);
    pthread_mutex_destroy(&mutex);
    return 0;
}