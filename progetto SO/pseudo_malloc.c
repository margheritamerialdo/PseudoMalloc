#include "pseudo_malloc.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>

#include "bit_map.h"
#include "buddy_allocator.h"

//definisco mem e buf
char mem[MAX_BUDDY_SIZE];
uint8_t buf[((1 << B_LEVELS)) - 1]; //numero di bit necessari per rappresentare un albero binario completo con n_levels.


void pseudoMalloc_init(buddy_allocator * b_alloc) {
    buddyAllocator_init(b_alloc, MIN_BUDDY_SIZE, B_LEVELS, mem, buf);

    printf("\n **** inizializzazione PSEUDO MALLOC eseguita **** \n \n");
}

void * pseudoMalloc_alloc(buddy_allocator * b_alloc, int size) {
    
    if (size <= 0) {
        printf("errore: dimensione blocco negativa o pari a 0\n");
        return NULL;
    }

    size += sizeof(int); //includere lo spazio per l'header dell'allocazione

    if (size <= PAGE_SIZE/4) {
        void * ptr_s = buddyAllocator_alloc(b_alloc, size);
        if (ptr_s == NULL) {
            printf("la funzione buddyAllocator_alloc ha fallito \n");
            return NULL;
        }
        printf("allocazione con buddyAllocator_alloc avvenuta con successo \n");
        return ptr_s;
    }
    else if (size > PAGE_SIZE/4){
        void * ptr_b = mmap(0, size, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        printf("allocazione con mmap avvenuta con successo \n");
        if (ptr_b == MAP_FAILED) {
            perror("Errore durante mmap");
            return NULL;
        }
        *((int*)ptr_b) = size;
        return ptr_b;
    }
    return NULL; 
}

void pseudoMalloc_free(buddy_allocator * b_alloc, void* ptr) {

    if (ptr == NULL) {
        printf("errore: richiesta non valida \n");
        return;
    }

    int i;
    void* first = mem;
    void* last = mem + MAX_BUDDY_SIZE * sizeof(char);

    if (ptr >= first && ptr <= last) {
        printf("free con buddy \n");
        buddyAllocator_free(b_alloc, ptr);
        printf("deallocazione con buddy riuscita \n\n");
    }
    
    else {
        int size = *((int*)ptr);

        i = munmap(ptr, size);
        if (i == -1) {
            perror("Errore durante munmap");
            return;
        }
         printf("deallocazione con munmap riuscita \n\n");
    }
}