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
// feel free to create additional structs/enums as necessary
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
struct address_list{
	uint32_t address;
	bool is_miss;
	struct address_list *list;
};

struct custom_data{
	int n;
	struct address_list *visited;
};

uint32_t custom_handle_mem_access(struct prefetcher *prefetcher, struct cache_system *cache_system,
                                  uint32_t address, bool is_miss)
{
    // perform the necessary prefetches for your custom strategy.
    if(((struct custom_data*) prefetcher->data)->n > 0){
	    for(int i = 1; i <= ((struct custom_data*) prefetcher->data)->n; i++){
		uint32_t fetch_address = address + i*cache_system->line_size;
    		cache_system_mem_access(cache_system, fetch_address, 'R', true);
	    }
    }
    int fetch_count = 0;
    //the last address where a miss occurred while traversing access linked list
    uint32_t last_missed = -1;
    //the last address where a misss occurred after the last time the current address was loaded into memory
    //is reset if a different address is the first one missed after the next time the current address was loaded into memeory
    uint32_t already_missed = -1;
    struct address_list* trav_list = ((struct custom_data*) prefetcher->data)->visited;

    while(fetch_count < 2 && trav_list->address != -1){
	    if(address == trav_list->address){
		    if(last_missed != -1 && last_missed == already_missed){
			    //printf("test\n");
			    fetch_count++;
			    cache_system_mem_access(cache_system, already_missed, 'R', true);
		    }
		    already_missed = last_missed;
		    last_missed = -1;
	    }else if(trav_list->is_miss){
		    last_missed = trav_list->address;
	    }
	    trav_list = trav_list->list;
    }

    struct address_list* new_address = malloc(sizeof(struct address_list));
    new_address->address = address;
    new_address->is_miss = is_miss;
    new_address->list = ((struct custom_data*) prefetcher->data)->visited;
    ((struct custom_data*) prefetcher->data)->visited = new_address;

    // Return the number of lines that were prefetched.
    return ((struct custom_data*) prefetcher->data)->n + fetch_count;
}

void custom_cleanup(struct prefetcher *prefetcher)
{
    // cleanup any additional memory that you allocated in the
    // custom_prefetcher_new function.
    while(((struct custom_data*) prefetcher->data)->visited->address != -1){
	    struct address_list *value = ((struct custom_data*) prefetcher->data)->visited;
	    ((struct custom_data*) prefetcher->data)->visited = ((struct custom_data*) prefetcher->data)->visited->list;
	    free(value);
    }
    free(((struct custom_data*) prefetcher->data)->visited);
    free(((struct custom_data*) prefetcher->data));
}

struct prefetcher *custom_prefetcher_new(uint32_t prefetch_amount)
{
    struct prefetcher *custom_prefetcher = calloc(1, sizeof(struct prefetcher));
    custom_prefetcher->handle_mem_access = &custom_handle_mem_access;
    custom_prefetcher->cleanup = &custom_cleanup;

    // TODO allocate any additional memory needed to store metadata here and
    // assign to custom_prefetcher->data.
    struct custom_data *new_data = malloc(sizeof(struct custom_data));
    new_data->n = prefetch_amount; 
    new_data->visited = (struct address_list*) malloc(sizeof(struct address_list));
    new_data->visited->address = -1;
    custom_prefetcher->data = new_data;
    

    return custom_prefetcher;
}
