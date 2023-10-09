#pragma once
#include "bit_map.h"

// definizione struttura dati per buddy allocator

typedef struct {
    int min_size; // dimensione minima che può assumero un blocco
    int n_levels; // livelli albero
    BitMap bitmap; 
    char* mem; // puntatore alla memoria allocata
} buddy_allocator;

//inizializzazione della struttura dati buddy allocator
void buddyAllocator_init(buddy_allocator* b_alloc, int min_size, int n_levels, char* mem, uint8_t *buf);

//cerca il primo blocco libero del livello passato
int buddyAllocator_free_block_at_level(buddy_allocator * b_alloc, int level);

//cerca il livello con la partizione più piccola che possa soddisfare la richiesta (data una dimensione)
int buddyAllocator_find_min_level(buddy_allocator * b_alloc, int size);

//cerca un blocco libero nel buddy allocator, scandendo tutti i livelli (a partire da level), altrimenti restituisce -1
int buddyAllocator_find_free_block(buddy_allocator * b_alloc, int level);

//calcola l'indirizzo di un blocco di memoria in base all'indice e al livello specificati
void * buddyAllocator_get_address(buddy_allocator * b_alloc, int index, int level);

//alloca un blocco del buddyAllocator di dimensione dim
void * buddyAllocator_alloc(buddy_allocator * b_alloc, int size);

//libera i buddy e nel caso sia possibile merge dei due buddy
void buddyAllocator_free_buddies(buddy_allocator * b_alloc, int index);

void buddyAllocator_free(buddy_allocator * b_alloc, void *ptr);
