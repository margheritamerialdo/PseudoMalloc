#pragma once
#include "buddy_allocator.h"

#define PAGE_SIZE 4096              //misura delle pagine più comunemente usata
#define MAX_BUDDY_SIZE 1024*1024    //(1MB)

//inizializzazione dell'allocatore
void pseudoMalloc_init(buddy_allocator * b_alloc, int n_levels, int min_size, uint8_t buf);

//alloca la memoria
void * pseudoMalloc_alloc(buddy_allocator * b_alloc, int size);

//dealloca la memoria
void pseudoMalloc_free(buddy_allocator * b_alloc, void* ptr, int size);
