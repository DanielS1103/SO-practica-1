#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>


int main(){
    pid_t pid;
    key_t key = 1234; // Shared memory key
    int shmId;
    float a, b, c, *ap;
    int status, cpid, r;

    shmId = shmget(key, 3*sizeof(float), 0666| IPC_CREAT);
    if(shmID < 0){
        perror("error creating shared memory");
        exit(-1);
    }
    ap = (float *) shmat(shmId, 0, 0);
    if(ap < 0){
    perror("error attaching shared memory");
    exit(-1);
    }
    *ap = 3.141592;

    pid = fork();

    if(pid < 0){
        perror("error en fork");
        exit(-1);
    }

    if(pid == 0){
        *(ap+1) = 0.707;
    }
    else{
        if(wait(&status) == pid){
            printf("/n %f  %f", *ap, *(ap+1));
        }
    }

    r = shmdt(ap);
    if(r < 0){
        perror("error en shmdt");
        exit(-1);
    }
    return 0;
}