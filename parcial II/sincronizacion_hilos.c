#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>


* proceso(void *datos){
    struct datos_tipo *datos_proceso;
    datos_proceso = (struct datos_tipo *) datos;
    int a, i, j, p;
    sem_wait(semaforo);
    a = datos_proceso -> dato;
    p = datos_proceso -> p;
    for (i=0; i <= p; i++){
        printf("%i")
    }

}