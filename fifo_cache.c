#include "fifo_cache.h"
#include <stdio.h>

FIFOCache* create_fifo_cache(int capacity) {
    FIFOCache *cache = (FIFOCache *)malloc(sizeof(FIFOCache));
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;
    return cache;
}

void free_fifo_cache(FIFOCache *cache) {
    FifoNode *current = cache->head;
    while (current) {
        FifoNode *next = current->next;
        free(current);
        current = next;
    }
    free(cache);
}

int find_in_fifo_cache(FIFOCache *cache, unsigned long key) {
    FifoNode *current = cache->head;
    while (current) {
        if (current->key == key) {
            return current->steps;
        }
        current = current->next;
    }
    return -1; // Not found
}

void add_to_fifo_cache(FIFOCache *cache, unsigned long key, int steps) {
    if (find_in_fifo_cache(cache, key) != -1) {
        return; // Already in cache, do not add again
    }

    FifoNode *new_node = (FifoNode *)malloc(sizeof(FifoNode));
    if (!new_node) {
        return;
    }
    new_node->key = key;
    new_node->steps = steps;
    new_node->next = NULL;

    if (!cache->head) {
        cache->head = new_node;
        cache->tail = new_node;
    } else {
        cache->tail->next = new_node;
        cache->tail = new_node;
    }

    if (cache->size < cache->capacity) {
        cache->size++;
    } else {
        remove_oldest_fifo_cache(cache);
    }
}

void remove_oldest_fifo_cache(FIFOCache *cache) {
    if (!cache->head) return; // No node to remove

    FifoNode *old_head = cache->head;
    cache->head = old_head->next;
    if (!cache->head) {
        cache->tail = NULL; // Reset tail if the list becomes empty
    }
    free(old_head);
    cache->size--;
}