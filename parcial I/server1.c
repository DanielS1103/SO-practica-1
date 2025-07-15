#include <stdio.h>      //
#include <stdlib.h>     // was throwing lots of notes and warnings without it
#include <string.h>
#include <unistd.h>     // to use fork function
#include <sys/wait.h>   // to use wait function
#include <sys/shm.h>    // to use shared memory
#include <netinet/in.h> // to define server struct
#include <sys/socket.h> // to use sockets
#include <strings.h> // to use bzero
#include <signal.h>     // to handle signals

#define PORT 3550
#define BACKLOG 8

int serverfd = -1; // make them global so handleInterruption can close()
int clientfd = -1;

void handleInterruption(int signalCode){
    printf("Exiting server\n");
    printf("Closing sockets\n");
    close(clientfd);
    close(serverfd);
    printf("Sockets closed\n");
    printf("Bye :)\n");
    exit(0);
}

int main() {
    char buffer[12];
    struct sigaction sa;
    struct sockaddr_in server, client;
    int r;
    size_t lenclient;

    //void     (*sa_handler)(int);
    //void     (*sa_sigaction)(int, siginfo_t *, void *); // extra info, returns who send the signal etc;
    //sigset_t   sa_mask;
    //int        sa_flags;
    //void     (*sa_restorer)(void);

    sa.sa_handler = handleInterruption;
    sigemptyset(&sa.sa_mask);  // Don't block any signals
    sa.sa_flags = 0;

    r = sigaction(SIGINT, &sa, NULL);
    if (r == -1){
        perror("Error on sigaction");
    }


    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd == -1){
        perror("Error crearing socket");
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;
    bzero(server.sin_zero, 8);

    r = bind(serverfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
    if (r == -1){
        perror("Error in bind");
    }

    r = listen(serverfd, BACKLOG);
    if(r == -1) {
        perror("Error in listen");
    }
    
    lenclient = sizeof(struct sockaddr_in);
    clientfd = accept(serverfd, (struct sockaddr *)&client, &lenclient);
    if (clientfd == -1){
        perror("Error accepting client");
    }

    r = send(clientfd, "Hola mundo", 10, 0);
    if (r!=11){
        perror("Error send");
    }

    r = recv(clientfd, buffer, 12, 0);
    if (r == -1) {
        perror("Error receiving data from server");
    }
    
    buffer[r] = 0;
    printf("%s\n", buffer);
    
    close(clientfd);
    close(serverfd);

    return 0;
}
