//
// This file defines the function signatures necessary for creating the three
// cache systems and defines the prefetcher struct.
//

#include "prefetchers.h"

// Nul Prefetcher
// ============================================================================
uint32_t null_handle_mem_access(struct prefetcher *prefetcher, struct cache_system *cache_system,
                                uint32_t address, bool is_miss)
{
    return 0; // No lines prefetched
}

void null_cleanup(struct prefetcher *prefetcher) {}

struct prefetcher *null_prefetcher_new()
{
    struct prefetcher *null_prefetcher = calloc(1, sizeof(struct prefetcher));
    null_prefetcher->handle_mem_access = &null_handle_mem_access;
    null_prefetcher->cleanup = &null_cleanup;
    return null_prefetcher;
}

// Sequential Prefetcher
// ============================================================================
// TODO feel free to create additional structs/enums as necessary
struct sequential_data{
	int n;
};

uint32_t sequential_handle_mem_access(struct prefetcher *prefetcher,
                                      struct cache_system *cache_system, uint32_t address,
                                      bool is_miss)
{
    if(((struct sequential_data*) prefetcher->data)->n > 0){
	    for(int i = 1; i <= ((struct sequential_data*) prefetcher->data)->n; i++){
		uint32_t fetch_address = address + i*cache_system->line_size;
    		cache_system_mem_access(cache_system, fetch_address, 'R', true);
	    }
    }
    // Return the number of lines that were prefetched.
    return ((struct sequential_data*) prefetcher->data)->n;
}

void sequential_cleanup(struct prefetcher *prefetcher)
{
    // cleanup any additional memory that you allocated in the
    // sequential_prefetcher_new function.
    free((struct sequential_data*) prefetcher->data);
}

struct prefetcher *sequential_prefetcher_new(uint32_t prefetch_amount)
{
    struct prefetcher *sequential_prefetcher = calloc(1, sizeof(struct prefetcher));
    sequential_prefetcher->handle_mem_access = &sequential_handle_mem_access;
    sequential_prefetcher->cleanup = &sequential_cleanup;

    // allocate any additional memory needed to store metadata here and
    // assign to sequential_prefetcher->data.
    struct sequential_data *new_data = malloc(sizeof(struct sequential_data));
    new_data->n = prefetch_amount;
    sequential_prefetcher->data = new_data;

    return sequential_prefetcher;
}

// Adjacent Prefetcher
// ============================================================================
uint32_t adjacent_handle_mem_access(struct prefetcher *prefetcher,
                                    struct cache_system *cache_system, uint32_t address,
                                    bool is_miss)
{
    // perform the necessary prefetches for the adjacent strategy.
    uint32_t fetch_address = address + cache_system->line_size;
    cache_system_mem_access(cache_system, fetch_address, 'R', true);
   
    // Return the number of lines that were prefetched.
    return 1;
}

void adjacent_cleanup(struct prefetcher *prefetcher)
{
    // cleanup any additional memory that you allocated in the
    // adjacent_prefetcher_new function.
}

struct prefetcher *adjacent_prefetcher_new()
{
    struct prefetcher *adjacent_prefetcher = calloc(1, sizeof(struct prefetcher));
    adjacent_prefetcher->handle_mem_access = &adjacent_handle_mem_access;
    adjacent_prefetcher->cleanup = &adjacent_cleanup;

    // allocate any additional memory needed to store metadata here and
    // assign to adjacent_prefetcher->data.

    return adjacent_prefetcher;
}

// Custom Prefetcher
// ============================================================================
uint32_t custom_handle_mem_access(struct prefetcher *prefetcher, struct cache_system *cache_system,
                                  uint32_t address, bool is_miss)
{
    // TODO perform the necessary prefetches for your custom strategy.

    // TODO: Return the number of lines that were prefetched.
    return 0;
}

void custom_cleanup(struct prefetcher *prefetcher)
{
    // TODO cleanup any additional memory that you allocated in the
    // custom_prefetcher_new function.
}

struct prefetcher *custom_prefetcher_new()
{
    struct prefetcher *custom_prefetcher = calloc(1, sizeof(struct prefetcher));
    custom_prefetcher->handle_mem_access = &custom_handle_mem_access;
    custom_prefetcher->cleanup = &custom_cleanup;

    // TODO allocate any additional memory needed to store metadata here and
    // assign to custom_prefetcher->data.

    return custom_prefetcher;
}
