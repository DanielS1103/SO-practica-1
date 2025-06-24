#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

void display_menu() {
    printf("\nBienvenido al sistema de búsqueda\n");
    printf("1. Buscar por slot\n");
    printf("2. Buscar por tx_idx\n");
    printf("3. Buscar por dirección\n");
    printf("4. Realizar búsqueda\n");
    printf("5. Salir\n");
    printf("Seleccione una opción: ");
}

void display_record(Record *rec) {
    printf("----------------------------------------\n");
    printf("Block Time: %s\n", rec->block_time);
    printf("Slot: %u\n", rec->slot);
    printf("Tx Index: %u\n", rec->tx_idx);
    printf("Direction: %s\n", rec->direction);
    printf("Base Coin: %s\n", rec->base_coin);
    printf("Base Amount: %llu\n", rec->base_coin_amount);
    printf("Quote Amount: %llu\n", rec->quote_coin_amount);
    printf("----------------------------------------\n");
}

int main() {
    unsigned int slot = 0;
    unsigned int tx_idx = 0;
    char direction[5] = "";
    int client_pid = getpid();
    
    // Crear tubería de respuesta
    char response_pipe[256];
    sprintf(response_pipe, RESPONSE_PIPE_TEMPLATE, client_pid);
    mkfifo(response_pipe, 0666);
    
    int option;
    do {
        display_menu();
        scanf("%d", &option);
        
        switch(option) {
            case 1:
                printf("Ingrese slot: ");
                scanf("%u", &slot);
                break;
            case 2:
                printf("Ingrese tx_idx: ");
                scanf("%u", &tx_idx);
                break;
            case 3:
                printf("Ingrese dirección (buy/sell): ");
                scanf("%4s", direction);
                break;
            case 4: {
                // Enviar solicitud
                int request_fd = open(REQUEST_PIPE, O_WRONLY);
                char request[256];
                sprintf(request, "client_pid=%d&slot=%u&tx_idx=%u&direction=%s", 
                        client_pid, slot, tx_idx, direction);
                write(request_fd, request, strlen(request) + 1);
                close(request_fd);
                
                // Recibir respuesta
                int response_fd = open(response_pipe, O_RDONLY);
                int count;
                read(response_fd, &count, sizeof(int));
                
                if (count == 0) {
                    printf("\nNA - No se encontraron resultados\n");
                } else {
                    Record *results = malloc(count * sizeof(Record));
                    read(response_fd, results, count * sizeof(Record));
                    
                    printf("\nResultados encontrados: %d\n", count);
                    for (int i = 0; i < count && i < 10; i++) {
                        printf("\nResultado %d:\n", i + 1);
                        display_record(&results[i]);
                    }
                    
                    if (count > 10) {
                        printf("\nMostrando 10 de %d resultados. Use filtros más específicos\n", count);
                    }
                    free(results);
                }
                close(response_fd);
                break;
            }
            case 5:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opción inválida\n");
        }
    } while (option != 5);
    
    unlink(response_pipe);
    return 0;
}