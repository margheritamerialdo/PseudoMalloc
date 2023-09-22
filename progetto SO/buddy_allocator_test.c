#include "bit_map.h"
#include "buddy_allocator.h"
#include "pseudo_malloc.h"
#include <stdio.h>

#define PAGE_SIZE 4096              //misura delle pagine più comunemente usata
#define MAX_BUDDY_SIZE 1024*1024    //(1MB)
#define B_LEVELS 9                  // visto negli esempi (source) del corso
#define MIN_BUDDY_SIZE (MAX_BUDDY_SIZE >> (B_LEVELS - 1)) //((MAX_BUDDY_SIZE/2^(B_LEVEL-1) = 4KB) valore più piccolo che il buddy allocator può gestire.
#define N_BUDDIES (1 << B_LEVELS) -1                      //numero di buddies nell'albero (2^B_LEVEL) - 1
#define BITSET_SIZE (N_BUDDIES + 7) >> 3 //dimensione del tuo bitset per tenere traccia dello stato dei blocchi

void main() {
    buddy_allocator b_alloc;
    uint8_t buf[N_BUDDIES];
    char mem[MAX_BUDDY_SIZE];
    
    buddyAllocator_init(&b_alloc, MIN_BUDDY_SIZE, B_LEVELS, mem, buf);
    
    //test per allocazione e deallocazione
    
    printf("TEST 1: allocazione blocco da 2048 \n");
    void *ptr1 = buddyAllocator_alloc(&b_alloc, 2048);  //troppo piccolo, la dimensione minima è 4096
    if (ptr1 == NULL) {
        printf("problema con l'allocazione blocco da 2048 \n \n");
    }
    else {
        printf("allocazione blocco da 2048 riuscita \n");
        buddyAllocator_free(&b_alloc, ptr1);
        printf("deallocazione blocco da 2048 riuscita \n \n");
    }
    printf("TEST 2: allocazione blocco da 4096 \n");
    void *ptr2 = buddyAllocator_alloc(&b_alloc, 4096); 
    if (ptr2 == NULL) {
        printf("problema con l'allocazione blocco da 4096 \n \n");
    }
    else {
        printf("allocazione blocco da 4096 riuscita \n");
        buddyAllocator_free(&b_alloc, ptr2);
        printf("deallocazione blocco da 4096 riuscita \n \n");
    }
    printf("TEST 3: allocazione blocco da 6144 \n");
    void *ptr3 = buddyAllocator_alloc(&b_alloc, 6144); 
    if (ptr3 == NULL) {
        printf("problema con l'allocazione blocco da 6144 \n \n");
    }
    else {
        printf("allocazione blocco da 6144 riuscita \n");
        buddyAllocator_free(&b_alloc, ptr3);
        printf("deallocazione blocco da 6144 riuscita \n \n");
    }

    printf("TEST 4: allocazione blocco da MAX_BUDDY_SIZE \n");
    void *ptr4 = buddyAllocator_alloc(&b_alloc, MAX_BUDDY_SIZE); 
    if (ptr4 == NULL) {
        printf("problema con l'allocazione blocco da MAX_BUDDY_SIZE \n \n");
    }
    else {
        printf("allocazione blocco da MAX_BUDDY_SIZE riuscita \n");
        buddyAllocator_free(&b_alloc, ptr4);
        printf("deallocazione blocco da MAX_BUDDY_SIZE riuscita \n \n");
    }
    
    //Test se supera la dimensione massima
    
  
}
