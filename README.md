
# 📄 Indexed CSV Search System

A high-performance C-based indexing and search system for large CSV datasets, designed for fast data retrieval using multiple search criteria with optimized binary file formats and inter-process communication.

## 🚀 Features

- **Multi-criteria Search**: Search by slot, transaction index, direction (buy/sell), wallet address, or row number
- **Binary Indexing**: Converts CSV data to optimized binary format for fast access
- **Hash Table Lookup**: O(1) average time complexity for slot+tx_idx combinations
- **Block-based Storage**: Efficient data organization using block indexing
- **Client-Server Architecture**: Uses named pipes (FIFOs) for inter-process communication
- **Memory Efficient**: Optimized for large datasets with minimal memory footprint

## 📁 Project Structure


## 🏗️ Architecture

### Data Structures

- **Record**: Main data structure containing transaction information (block time, slot, tx_idx, wallet, direction, amounts, etc.)
- **BlockIndex**: Organizes records into blocks for efficient range queries
- **HashTable**: Provides O(1) lookup for slot+tx_idx combinations
- **Metadata**: Stores dataset statistics and configuration

### Search Types

1. **SEARCH_BY_SLOT**: Find records by slot number
2. **SEARCH_BY_TX_IDX**: Find records by transaction index
3. **SEARCH_BY_DIRECTION**: Find records by transaction direction (buy/sell)
4. **SEARCH_BY_WALLET**: Find records by wallet address
5. **SEARCH_BY_ROW**: Direct access to specific row number

### Communication Protocol

The system uses named pipes for client-server communication:
- **Request Pipe**: `/tmp/search_request` - Clients send search requests
- **Response Pipe**: `/tmp/search_response_{PID}` - Server sends results back

## 🛠️ Building the Project

### Prerequisites
- GCC compiler with C11 support
- POSIX-compatible system (Linux/Unix)
- Make utility

### Compilation
```bash
make all
```

Individual components:
```bash
make preprocess    # Build preprocessing utility
make search_server # Build search server
make client       # Build client application
```

### Clean Build
```bash
make clean
```

## 📊 Usage

### 1. Data Preprocessing
Convert your CSV dataset to optimized binary format:

```bash
make preprocess-data
# or manually:
./preprocess your_dataset.csv
```

This creates:
- `data.bin`: Binary records
- `slot_index.bin`: Block index
- `metadata.bin`: Dataset metadata
- `hashtable.bin`: Hash table for fast lookups

### 2. Start the Search Server
```bash
make run-server
# or manually:
./search_server
```

### 3. Run the Client
In another terminal:
```bash
make run-client
# or manually:
./client
```

## 🎯 Client Interface

The client provides an interactive menu:

```
Search System
1. Select first criteria
2. Select second criteria
3. Perform search
4. Exit
```

### Search Criteria Options:
1. **Slot**: Search by slot number
2. **Tx_idx**: Search by transaction index
3. **Direction**: Search by buy/sell direction
4. **Wallet**: Search by wallet address
5. **Row**: Direct access by row number

### Example Search Flow:
1. Select option 1 to set first criteria (e.g., Slot = 12345)
2. Select option 2 to set second criteria (e.g., Direction = "buy")
3. Select option 3 to execute search
4. View results (up to 10 displayed)

## ⚡ Performance Optimizations

### Indexing Strategy
- **Hash Table**: O(1) lookup for slot+tx_idx combinations
- **Block Index**: Reduces I/O by organizing data into blocks
- **Binary Format**: Eliminates CSV parsing overhead

### Search Algorithms
- **Combined Criteria**: Supports AND operations between criteria
- **Optimized Scans**: Block-level filtering reduces search space
- **Memory Management**: Dynamic result allocation with realloc optimization

### System Optimizations
- **Compiler Flags**: `-O3` optimization for maximum performance
- **Block Size**: 5000 records per block for optimal I/O
- **Hash Size**: Prime number (1,000,003) for even distribution

## 🔧 Configuration

### Key Constants (common.h)
```c
#define BLOCK_SIZE 5000        // Records per block
#define HASH_SIZE 1000003      // Hash table size (prime)
#define MAX_MEMORY 10MB        // Memory limit
```

### File Paths
- Data files stored in current directory
- Pipes created in `/tmp/` directory
- Configurable via `#define` constants

## 📈 Performance Characteristics

- **Binary Search**: O(log n) for hash table lookups
- **Block Scanning**: O(n/block_size) for filtered searches
- **Memory Usage**: ~10MB limit with dynamic allocation
- **Concurrency**: Multiple clients supported via separate response pipes

## 🛡️ Error Handling

- **Signal Handling**: Graceful cleanup on SIGINT/SIGTERM
- **File Validation**: Comprehensive error checking for I/O operations
- **Memory Management**: Proper allocation/deallocation with error recovery
- **Pipe Communication**: Robust error handling for IPC operations

## 🔍 Search Examples

### Single Criteria Search
- Find all transactions for slot 12345
- Find all "buy" transactions
- Get record at row 1000

### Combined Criteria Search
- Slot = 12345 AND Direction = "buy"
- Wallet = "ABC123..." AND Tx_idx = 5
- Any combination of available criteria

## 📝 Data Format

### Input CSV Format
Expected columns:
```
block_time,slot,tx_idx,signing_wallet,direction,base_coin,base_coin_amount,
quote_coin_amount,virtual_token_balance_after,virtual_sol_balance_after,
signature,provided_gas_fee,provided_gas_limit,fee,consumed_gas
```

### Binary Record Format
Optimized fixed-size structure for fast I/O and indexing.

## 🤝 Contributing

1. Follow C11 standard
2. Maintain POSIX compatibility
3. Add error handling for new features
4. Update documentation for API changes
5. Test with large datasets

## 📄 License

This project is part of an educational dataset indexing system.
