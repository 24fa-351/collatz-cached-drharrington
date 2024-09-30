#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <stdlib.h>

// Define a node for the LRU cache
typedef struct CacheNode {
    unsigned long key;
    int steps;
    struct CacheNode *next;
    struct CacheNode *prev;
} CacheNode;

// Define the LRU cache structure
typedef struct {
    int capacity;
    int size;
    CacheNode *head;
    CacheNode *tail;
    // You can use a hash table to store nodes for quick access
} LRUCache;

// Function prototypes
LRUCache* create_lru_cache(int capacity);
void free_lru_cache(LRUCache *cache);
int find_in_lru_cache(LRUCache *cache, unsigned long key);
void add_to_lru_cache(LRUCache *cache, unsigned long key, int steps);
void update_lru_cache(LRUCache *cache, CacheNode *node);
void remove_least_recently_used(LRUCache *cache);

#endif // LRU_CACHE_H