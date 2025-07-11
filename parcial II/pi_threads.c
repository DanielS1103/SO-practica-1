#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>

#define NUM_THREADS 4
#define NUM_TERMS 10000000

double pi = 0.0;
sem_t sem;

void* calcular_parte(void* arg) {
    long thread_id = (long)arg;
    double local_sum = 0.0;

    for (long i = thread_id; i < NUM_TERMS; i += NUM_THREADS) {
        double term = (i % 2 == 0 ? 1.0 : -1.0) / (2.0 * i + 1.0);
        local_sum += term;
    }

    sem_wait(&sem);  // entrada a la sección crítica
    pi += local_sum;
    sem_post(&sem);  // salida de la sección crítica

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    sem_init(&sem, 0, 1);  // inicializa el semáforo con valor 1

    // Crear hilos
    for (long i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, calcular_parte, (void*)i)) {
            fprintf(stderr, "Error al crear hilo %ld\n", i);
            return 1;
        }
    }

    // Esperar a que terminen los hilos
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pi *= 4.0;
    printf("Valor aproximado de π: %.15f\n", pi);

    sem_destroy(&sem);
    return 0;
}
