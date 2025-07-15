#include <stdio.h>
#include <unistd.h>
#include <sys/wait>
#include <stdlib.h>


int main(){
    int fd[2];

    if(pipe(fd) == -1{
        perror();
        return 1;
    }

    pid_t pid = fork();

    if(pid == -1){
        perror();
        return 1;
    }

    else if (pid == 0){ //hijo
        close(fd[0]);

        int suma_hijo = 0;

        for(int i = 0; i <= 499; i++){
            suma_hijo += i;
        }

        printf("la suma del hijo es: %d", suma_hijo);

        write(fd[1], &suma_hijo, sizeof(suma_hijo));
        close(fd[1]);

        exit(0);
    }

    else{
        close(fd[1]);

        int suma_padre = 0;

        for(int i = 500; i <= 999; i++){
            suma_padre += i;
        }

        wait(NULL);

        int suma_hijo;
        read(fd[0], &suma_hijo, sizeof(suma_hijo));
        close(fd[0]);

        printf("la suma del padre es: %d");
        printf("la suma total es: %d", suma_hijo + suma_padre);
    }
    return 0;
}