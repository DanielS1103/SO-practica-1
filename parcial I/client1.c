#include <stdio.h>      //
#include <stdlib.h>     // was throwing lots of notes and warnings without it
#include <string.h>
#include <unistd.h>     // to use fork function
#include <sys/wait.h>   // to use wait function
#include <sys/shm.h>    // to use shared memory
#include <netinet/in.h> // to define 
#include <sys/socket.h> // to use sockets
#include <strings.h> // to use bzero
#include <signal.h>     // to handle signals

#define PORT 3550
#define BACKLOG 8

int fd = -1;

/*
void handleInterruption(int signalCode){
    printf("Exiting client\n");
    printf("Closing socket\n");
    close(fd);
    printf("Socket closed\n");
    printf("Bye :)\n");
    exit(0);
}
*/

int main() {
    // Client side
    // struct sigaction sa; 
    char buffer[10];
    struct sockaddr_in server;
    int fd, r;

    /*
    sa.sa_handler = handleInterruption;
    sigemptyset(&sa.sa_mask);  // Don't block any signals
    sa.sa_flags = 0;
    r = sigaction(SIGINT, &sa, NULL);
    if (r == -1){
        perror("Error on sigaction");
    }
    */
    
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1){
        perror("Error crearing socket");
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    bzero(server.sin_zero, 8);

    r = connect(fd, (struct sockaddr*)&server, sizeof(struct sockaddr));
    if (r == -1) {
        perror("Error connecting to server");
    }

    r = recv(fd, buffer, 11, 0); // es bloqueante
    if (r == -1) {
        perror("Error receiving data from server");
    }
    
    buffer[r] = 0;
    printf("%s\n", buffer);
    
    r = send(fd, "hola server", 11, 0);
    if (r != 11) {
        perror("Error sending data to server");
    }
    
    close(fd);

    return 0;
}
