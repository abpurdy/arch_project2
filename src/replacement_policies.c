//
// This file contains all of the implementations of the replacement_policy
// constructors from the replacement_policies.h file.
//
// It also contains stubs of all of the functions that are added to each
// replacement_policy struct at construction time.
//
// ============================================================================
// NOTE: It is recommended that you read the comments in the
// replacement_policies.h file for further context on what each function is
// for.
// ============================================================================
//

#include "replacement_policies.h"
#include <limits.h>

// LRU Replacement Policy
// ============================================================================
// feel free to create additional structs/enums as necessary
struct lru_data{
	int count;
	int *use_time;
};

void lru_cache_access(struct replacement_policy *replacement_policy,
                      struct cache_system *cache_system, uint32_t set_idx, uint32_t tag)
{
    //update the LRU replacement policy state given a new memory access
    for(int i = 0; i < cache_system->associativity; i++){
	    if(cache_system->cache_lines[set_idx*cache_system->associativity+i].tag == tag){
		    ((struct lru_data*) replacement_policy->data)->use_time[set_idx*cache_system->associativity+i] = ((struct lru_data*) 
			    replacement_policy->data)->count;
	    }
    }
    ((struct lru_data*) replacement_policy->data)->count++;
}

uint32_t lru_eviction_index(struct replacement_policy *replacement_policy,
                            struct cache_system *cache_system, uint32_t set_idx)
{
    // return the index within the set that should be evicted.
    int oldest_time = ((struct lru_data*) replacement_policy->data)->use_time[set_idx*cache_system->associativity];
    int oldest_index = 0;
    for(int i = 1; i < cache_system->associativity; i++){
	    if(((struct lru_data*) replacement_policy->data)->use_time[set_idx*cache_system->associativity + i] < oldest_time){
		    oldest_index = i;
		    oldest_time = ((struct lru_data*) replacement_policy->data)->use_time[set_idx*cache_system->associativity + i];
	    }
    }

    return (uint32_t) oldest_index;
}

void lru_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    // cleanup any additional memory that you allocated in the
    // lru_replacement_policy_new function.
    free(((struct lru_data*) replacement_policy->data)->use_time);
    free(((struct lru_data*) replacement_policy->data));
}

struct replacement_policy *lru_replacement_policy_new(uint32_t sets, uint32_t associativity)
{
    struct replacement_policy *lru_rp = calloc(1, sizeof(struct replacement_policy));
    lru_rp->cache_access = &lru_cache_access;
    lru_rp->eviction_index = &lru_eviction_index;
    lru_rp->cleanup = &lru_replacement_policy_cleanup;

    // allocate any additional memory to store metadata here and assign to
    // lru_rp->data.
    struct lru_data *new_data = malloc(sizeof(struct lru_data));
    new_data->count = 0;
    new_data->use_time = calloc(sets*associativity, sizeof(int));
    lru_rp->data = new_data;

    return lru_rp;
}

// RAND Replacement Policy
// ============================================================================
void rand_cache_access(struct replacement_policy *replacement_policy,
                       struct cache_system *cache_system, uint32_t set_idx, uint32_t tag)
{
    // update the RAND replacement policy state given a new memory access
}

uint32_t rand_eviction_index(struct replacement_policy *replacement_policy,
                             struct cache_system *cache_system, uint32_t set_idx)
{
    // return the index within the set that should be evicted.

    return (rand() % cache_system->associativity);
}

void rand_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    // cleanup any additional memory that you allocated in the
    // rand_replacement_policy_new function.
}

struct replacement_policy *rand_replacement_policy_new(uint32_t sets, uint32_t associativity)
{
    // Seed randomness
    srand(time(NULL));

    struct replacement_policy *rand_rp = malloc(sizeof(struct replacement_policy));
    rand_rp->cache_access = &rand_cache_access;
    rand_rp->eviction_index = &rand_eviction_index;
    rand_rp->cleanup = &rand_replacement_policy_cleanup;

    // allocate any additional memory to store metadata here and assign to
    // rand_rp->data.

    return rand_rp;
}

// LRU_PREFER_CLEAN Replacement Policy
// ============================================================================
void lru_prefer_clean_cache_access(struct replacement_policy *replacement_policy,
                                   struct cache_system *cache_system, uint32_t set_idx,
                                   uint32_t tag)
{
    // update the LRU_PREFER_CLEAN replacement policy state given a new
    // memory access
    // NOTE: you may be able to share code with the LRU policy
    for(int i = 0; i < cache_system->associativity; i++){
	    if(cache_system->cache_lines[set_idx*cache_system->associativity+i].tag == tag)
		    ((struct lru_data*) replacement_policy->data)->use_time[set_idx*cache_system->associativity+i] = ((struct lru_data*) 
			    replacement_policy->data)->count;
    }
    ((struct lru_data*) replacement_policy->data)->count++;
}

uint32_t lru_prefer_clean_eviction_index(struct replacement_policy *replacement_policy,
                                         struct cache_system *cache_system, uint32_t set_idx)
{
    // return the index within the set that should be evicted.
    int oldest_time = INT_MAX;
    int oldest_index = -1;
    for(int i = 0; i < cache_system->associativity; i++){
	    if(((struct lru_data*) replacement_policy->data)->use_time[set_idx*cache_system->associativity + i] < oldest_time &&
			    cache_system->cache_lines[set_idx*cache_system->associativity+i].status != MODIFIED){
		    oldest_index = i;
		    oldest_time = ((struct lru_data*) replacement_policy->data)->use_time[set_idx*cache_system->associativity + i];
	    }
    }
    if(oldest_index == -1){
	    for(int i = 0; i < cache_system->associativity; i++){
		    if(((struct lru_data*) replacement_policy->data)->use_time[set_idx*cache_system->associativity + i] < oldest_time){
			    oldest_index = i;
			    oldest_time = ((struct lru_data*) replacement_policy->data)->use_time[set_idx*cache_system->associativity + i];
		    }
	    }
    }

    return (uint32_t) oldest_index;
}

void lru_prefer_clean_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    // cleanup any additional memory that you allocated in the
    // lru_prefer_clean_replacement_policy_new function.
    free(((struct lru_data*) replacement_policy->data)->use_time);
    free(((struct lru_data*) replacement_policy->data));


}

struct replacement_policy *lru_prefer_clean_replacement_policy_new(uint32_t sets,
                                                                   uint32_t associativity)
{
    struct replacement_policy *lru_prefer_clean_rp = malloc(sizeof(struct replacement_policy));
    lru_prefer_clean_rp->cache_access = &lru_prefer_clean_cache_access;
    lru_prefer_clean_rp->eviction_index = &lru_prefer_clean_eviction_index;
    lru_prefer_clean_rp->cleanup = &lru_prefer_clean_replacement_policy_cleanup;

    // allocate any additional memory to store metadata here and assign to
    // lru_prefer_clean_rp->data.
    struct lru_data *new_data = malloc(sizeof(struct lru_data));
    new_data->count = 0;
    new_data->use_time = calloc(sets*associativity, sizeof(int));
    lru_prefer_clean_rp->data = new_data;


    return lru_prefer_clean_rp;
}
