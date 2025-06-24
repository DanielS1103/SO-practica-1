#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <stdint.h>

#define DATA_FILE "data.bin"
#define HASH_INDEX_FILE "hash_index.bin"
#define SLOT_INDEX_FILE "slot_index.bin"
#define METADATA_FILE "metadata.bin"
#define MAX_MEMORY 10 * 1024 * 1024  // 10MB
#define HASH_SIZE 2000000
#define REQUEST_PIPE "/tmp/search_request"
#define RESPONSE_PIPE_TEMPLATE "/tmp/search_response_%d"

typedef struct {
    char block_time[20]; // "YYYY-MM-DD HH:MM:SS"
    unsigned int slot;
    unsigned int tx_idx;
    char signing_wallet[50];
    char direction[5];   // "buy" o "sell"
    char base_coin[100];
    unsigned long long base_coin_amount;
    unsigned long long quote_coin_amount;
    unsigned long long virtual_token_balance_after;
    unsigned long long virtual_sol_balance_after;
    char signature[100];
    unsigned long provided_gas_fee;
    unsigned long provided_gas_limit;
    unsigned long fee;
    unsigned long consumed_gas;
} Record;

typedef struct {
    unsigned int slot;
    unsigned int tx_idx;
    long offset;
} HashEntry;

typedef struct {
    unsigned int min_slot;
    unsigned int max_slot;
    long offset;
} BlockIndex;

typedef struct {
    unsigned int record_count;
    unsigned int block_count;
    size_t record_size;
} Metadata;

#endif // COMMON_H