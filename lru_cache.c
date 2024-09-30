#include "lru_cache.h"
#include <stdio.h>

LRUCache* create_lru_cache(int capacity) {
    LRUCache *cache = (LRUCache *)malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;
    return cache;
}

void free_lru_cache(LRUCache *cache) {
    CacheNode *current = cache->head;
    while (current) {
        CacheNode *next = current->next;
        free(current);
        current = next;
    }
    free(cache);
}

int find_in_lru_cache(LRUCache *cache, unsigned long key) {
    CacheNode *current = cache->head;
    while (current) {
        if (current->key == key) {
            update_lru_cache(cache, current);
            return current->steps;
        }
        current = current->next;
    }
    return -1; // Not found
}

void add_to_lru_cache(LRUCache *cache, unsigned long key, int steps) {
    // Check if the key is already present
    if (find_in_lru_cache(cache, key) != -1) {
        return; // Already in cache, do not add again
    }

    CacheNode *new_node = (CacheNode *)malloc(sizeof(CacheNode));
    if (!new_node) {
        return;
    }
    new_node->key = key;
    new_node->steps = steps;
    new_node->next = NULL;
    new_node->prev = NULL;

    // Add to the front of the list
    if (!cache->head) {
        cache->head = new_node;
        cache->tail = new_node;
    } else {
        new_node->next = cache->head;
        cache->head->prev = new_node;
        cache->head = new_node;
    }

    // If over capacity, remove the least recently used
    cache->size++;
    if (cache->capacity > 0 && cache->size > cache->capacity) {
        remove_least_recently_used(cache);
    }
}

void update_lru_cache(LRUCache *cache, CacheNode *node) {
    if (node == cache->head) {
        return; // Already the most recently used
    }

    // Remove node from its current position
    if (node->prev) {
        node->prev->next = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    }
    if (node == cache->tail) {
        cache->tail = node->prev;
    }

    // Move it to the front
    node->next = cache->head;
    node->prev = NULL;
    if (cache->head) {
        cache->head->prev = node;
    }
    cache->head = node;

    if (!cache->tail) {
        cache->tail = node;
    }
}

void remove_least_recently_used(LRUCache *cache) {
    if (!cache->tail) return; // No node to remove

    CacheNode *old_tail = cache->tail;
    cache->tail = old_tail->prev;
    if (cache->tail) {
        cache->tail->next = NULL;
    } else {
        cache->head = NULL; // Cache is now empty
    }
    free(old_tail);
    cache->size--;
}