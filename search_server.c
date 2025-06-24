#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

Metadata meta;
BlockIndex *block_index;
int data_fd, hash_fd, slot_fd;

void cleanup(int sig) {
    printf("\nCleaning up resources...\n");
    munmap(block_index, meta.block_count * sizeof(BlockIndex));
    close(data_fd);
    close(hash_fd);
    close(slot_fd);
    unlink(REQUEST_PIPE);
    exit(0);
}

void search_records(unsigned int slot, unsigned int tx_idx, char *direction, 
                   Record **results, int *count) {
    *count = 0;
    *results = NULL;
    
    // Buscar en el índice de bloques
    int block_found = 0;
    for (unsigned int i = 0; i < meta.block_count; i++) {
        if (slot >= block_index[i].min_slot && slot <= block_index[i].max_slot) {
            block_found = 1;
            // Leer bloque de datos
            lseek(data_fd, block_index[i].offset, SEEK_SET);
            int block_size = (i == meta.block_count - 1) ? 
                (meta.record_count % 1000) : 1000;
            Record *block = malloc(block_size * sizeof(Record));
            read(data_fd, block, block_size * sizeof(Record));
            
            // Filtrar registros
            for (int j = 0; j < block_size; j++) {
                if (block[j].slot == slot && 
                    (tx_idx == 0 || block[j].tx_idx == tx_idx) &&
                    (direction[0] == '\0' || strcmp(block[j].direction, direction) == 0)) {
                    *results = realloc(*results, (*count + 1) * sizeof(Record));
                    (*results)[*count] = block[j];
                    (*count)++;
                }
            }
            free(block);
        }
    }
    
    if (!block_found) {
        *count = 0;
    }
}

int main() {
    // Registrar manejador de señales
    signal(SIGINT, cleanup);
    signal(SIGTERM, cleanup);
    
    // Cargar metadatos
    int meta_fd = open(METADATA_FILE, O_RDONLY);
    read(meta_fd, &meta, sizeof(Metadata));
    close(meta_fd);
    
    // Abrir archivos de datos
    data_fd = open(DATA_FILE, O_RDONLY);
    hash_fd = open(HASH_INDEX_FILE, O_RDONLY);
    slot_fd = open(SLOT_INDEX_FILE, O_RDONLY);
    
    // Mapear índice de bloques a memoria
    block_index = mmap(NULL, meta.block_count * sizeof(BlockIndex), 
                      PROT_READ, MAP_SHARED, slot_fd, 0);
    
    // Crear tubería de solicitud
    mkfifo(REQUEST_PIPE, 0666);
    
    printf("Search Server Started (PID: %d)\n", getpid());
    printf("Memory usage: %.2f MB\n", 
           (meta.block_count * sizeof(BlockIndex)) / (1024.0 * 1024.0));
    
    while (1) {
        int request_fd = open(REQUEST_PIPE, O_RDONLY);
        char request[256];
        read(request_fd, request, sizeof(request));
        close(request_fd);
        
        // Parsear solicitud: client_pid=X&slot=Y&tx_idx=Z&direction=A
        int client_pid;
        unsigned int slot = 0, tx_idx = 0;
        char direction[5] = "";
        sscanf(request, "client_pid=%d&slot=%u&tx_idx=%u&direction=%4s", 
               &client_pid, &slot, &tx_idx, direction);
        
        // Realizar búsqueda
        Record *results = NULL;
        int count = 0;
        search_records(slot, tx_idx, direction, &results, &count);
        
        // Preparar tubería de respuesta
        char response_pipe[256];
        sprintf(response_pipe, RESPONSE_PIPE_TEMPLATE, client_pid);
        int response_fd = open(response_pipe, O_WRONLY);
        
        // Enviar respuesta
        write(response_fd, &count, sizeof(int));
        if (count > 0) {
            write(response_fd, results, count * sizeof(Record));
            free(results);
        }
        close(response_fd);
    }
    
    return 0;
}