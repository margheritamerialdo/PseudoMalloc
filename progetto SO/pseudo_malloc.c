#include "pseudo_malloc.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "bit_map.h"
#include "buddy_allocator.h"

//definisco delle variabili globali e strutture
char mem[MAX_BUDDY_SIZE];
uint8_t buf[((1 << (B_LEVELS)) -1)]; // valore massimo rappresentabile con B_LEVELS numero di bit.


void pseudoMalloc_init(buddy_allocator * b_alloc) {
     buddyAllocator_init(b_alloc, MIN_BUDDY_SIZE, B_LEVELS, mem, buf);
}

void * pseudoMalloc_alloc(buddy_allocator * b_alloc, int size) {

    assert(size > 0);

    size += sizeof(int); //includere lo spazio per l'header dell'allocazione

    if (size <= PAGE_SIZE/4) {
        void * ptr_s = buddyAllocator_alloc(b_alloc, size);
        if (ptr_s == NULL) {
            printf("la funzione buddyAllocator_alloc ha fallito");
            return NULL;
        }
        return ptr_s;
    }
    else if (size > PAGE_SIZE/4){
        void * ptr_b = mmap(0, size, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (ptr_b == MAP_FAILED) {
            perror("errore allocazione memoria con mmap \n");
            return NULL;
        }
        return ptr_b;
    }
}

void pseudoMalloc_free(buddy_allocator * b_alloc, void* ptr, int size) {

    if (size < 1 || ptr == NULL) {
        printf("errore: richiesta non valida");
        return;
    }

    size += sizeof(int);
    int i;

    if (size <= PAGE_SIZE/4) {
        buddyAllocator_free(b_alloc, ptr);
    }

    else if (size > PAGE_SIZE/4) {
        i = munmap(ptr, size);
        if (i == -1) {
            perror("errore durante munmap");
            return;
        }
    }
}
