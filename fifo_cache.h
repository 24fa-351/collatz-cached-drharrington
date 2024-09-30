#ifndef FIFO_CACHE_H
#define FIFO_CACHE_H

#include <stdlib.h>

// Define a node for the FIFO cache
typedef struct FifoNode {
    unsigned long key;
    int steps;
    struct FifoNode *next;
} FifoNode;

// Define the FIFO cache structure
typedef struct {
    int capacity;
    int size;
    FifoNode *head;
    FifoNode *tail;
} FIFOCache;

// Function prototypes
FIFOCache* create_fifo_cache(int capacity);
void free_fifo_cache(FIFOCache *cache);
int find_in_fifo_cache(FIFOCache *cache, unsigned long key);
void add_to_fifo_cache(FIFOCache *cache, unsigned long key, int steps);
void remove_oldest_fifo_cache(FIFOCache *cache);

#endif // FIFO_CACHE_H