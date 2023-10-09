#pragma once
#include "buddy_allocator.h"

#define PAGE_SIZE 4096              //misura delle pagine più comunemente usata
#define MAX_BUDDY_SIZE 1024*1024    //(1MB)
#define B_LEVELS 9                  // visto negli esempi (source) del corso
#define MIN_BUDDY_SIZE (MAX_BUDDY_SIZE >> (B_LEVELS - 1)) //((MAX_BUDDY_SIZE/2^(B_LEVEL-1) = 4KB) valore più piccolo che il buddy allocator può gestire.

//inizializzazione dell'allocatore
void pseudoMalloc_init(buddy_allocator * b_alloc);

//alloca la memoria
void * pseudoMalloc_alloc(buddy_allocator * b_alloc, int size);

//dealloca la memoria
void pseudoMalloc_free(buddy_allocator * b_alloc, void* ptr);