#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>

#define NUM_THREADS 4
#define NUM_TERMS 100000000

double fac = 0.0;
sem_t sem;

unsigned long long factorial(unsigned int n){
    if(n == 0) return 1;
    return n * factorial(n-1);
}

int main(){
    unsigned int numero;
    printf("ingrese un numero positivo: ");
    if(scanf("%u", &numero) != 1){
        printf("entrada invalida. \n");
        return 1;
    }

    printf("el facotorial de %u es: %llu\n", numero, factorial(numero));
    return 0;  
}