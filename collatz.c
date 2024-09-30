#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "lru_cache.h"
#include "fifo_cache.h"

typedef enum
{
    NONE,
    LRU,
    FIFO
} CachePolicy;

CachePolicy parse_cache_policy(char *policy_str)
{
    if (strcmp(policy_str, "none") == 0)
    {
        return NONE;
    }
    else if (strcmp(policy_str, "LRU") == 0)
    {
        return LRU;
    }
    else if (strcmp(policy_str, "FIFO") == 0)
    {
        return FIFO;
    }
    fprintf(stderr, "Unknown cache policy: %s. Using 'none'.\n", policy_str);
    return NONE;
}

// Function to perform the Collatz conjecture and return the number of steps
int collatz_steps(unsigned long num)
{
    int steps = 0;
    while (num != 1)
    {
        if (num % 2 == 0)
        {
            num /= 2;
        }
        else
        {
            num = 3 * num + 1;
            steps++;
        }
    }
    return steps;
}

// Wrapper function with caching
int collatz_steps_with_cache(unsigned long n, void *cache, CachePolicy cache_policy, int *hits, int *checks)
{
    int steps = -1;

    if (cache_policy == LRU)
    {
        // Check cache for LRU
        steps = find_in_lru_cache((LRUCache *)cache, n);
        (*checks)++;
        if (steps == -1)
        {
            steps = collatz_steps(n);
            add_to_lru_cache((LRUCache *)cache, n, steps);
        } else {
            (*hits)++;
        }
    }
    else if (cache_policy == FIFO)
    {
        // Check cache for FIFO
        steps = find_in_fifo_cache((FIFOCache *)cache, n);
        (*checks)++;
        if (steps == -1)
        {
            steps = collatz_steps(n);
            add_to_fifo_cache((FIFOCache *)cache, n, steps);
        }
        else
        {
            (*hits)++;
        }
    }
    else
    {
        // No cache
        steps = collatz_steps(n);
    }

    return steps;
}

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        fprintf(stderr, "Usage: %s <Number of values to test> <Smallest value> <Largest value> <Cache policy> <Cache size>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);                                  
    int MIN = atoi(argv[2]);                                
    int MAX = atoi(argv[3]);                                
    CachePolicy cache_policy = parse_cache_policy(argv[4]); 
    int cache_capacity = atoi(argv[5]);                     

    if (MIN > MAX || N <= 0 || MIN <= 0 || cache_capacity <= 0)
    {
        fprintf(stderr, "Invalid input: Make sure N > 0, MIN > 0, and MIN <= MAX, and Cache capacity > 0.\n");
        return 1;
    }

    // Initialize cache based on policy
    LRUCache *lru_cache = NULL;
    FIFOCache *fifo_cache = NULL;
    if (cache_policy == LRU)
    {
        lru_cache = create_lru_cache(cache_capacity);
    }
    else if (cache_policy == FIFO)
    {
        fifo_cache = create_fifo_cache(cache_capacity);
    }
    else
    {
        fprintf(stderr, "Invalid cache policy: %d\n", cache_policy);
        return 1;
    }

    // Open the output CSV file
    FILE *file = fopen("collatz_output.csv", "w");
    if (!file)
    {
        perror("Failed to open file");
        if (lru_cache)
        {
            free_lru_cache(lru_cache);
        }
        if (fifo_cache)
        {
            free_fifo_cache(fifo_cache);
        }
        return 1;
    }

    // Write CSV headers
    fprintf(file, "Cache Hit %%: XX.XX%%\n");
    fprintf(file, "RandomNumber,Steps\n");

    // Seed the random number generator
    srand(time(NULL));

    // Generate N random numbers and calculate Collatz steps
    int hits = 0, checks = 0;
    for (int i = 0; i < N; i++)
    {
        unsigned long RN = (rand() % (MAX - MIN + 1)) + MIN;

        // Use the wrapper function for caching
        int steps = collatz_steps_with_cache(RN,
                                             (cache_policy == LRU ? (void *)lru_cache : (void *)fifo_cache),
                                             cache_policy, &hits, &checks);

        // Output the result as CSV
        fprintf(file, "%lu,%d\n", RN, steps);
    }

    double hit_percentage = checks > 0 ? (double)hits / checks * 100.0 : 0.0;
    fseek(file, 0, SEEK_SET);
    fprintf(file, "Cache Hit %%: %.2f%%\n", hit_percentage);
    fclose(file);

    // Free caches before exit
    if (lru_cache)
    {
        free_lru_cache(lru_cache);
    }
    if (fifo_cache)
    {
        free_fifo_cache(fifo_cache);
    }

    printf("Collatz conjecture results with caching policy '%s' written to 'collatz_output.csv'\n", argv[4]);

    return 0;
}