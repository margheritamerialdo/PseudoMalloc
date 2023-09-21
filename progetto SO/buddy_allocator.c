#include "buddy_allocator.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>

int get_parent_idx(int index) {
    return (int)floor((index - 1)/2);
}

int get_letf_child_idx(int index) {
    return (int)(index * 2 + 1);
}

int get_right_child_idx(int index) {
    return (int)(index * 2 + 2);
}

int get_buddy_idx(int index) {
    if (index == 0)
        return -1; //la radice non ha buddy
    else if (index % 2 == 0) 
        return index - 1;
    else {
        return index + 1;
    }
}

get_level_from_index(int index) {
    return floor(log2(index));
}

get_first_idx_at_level(int level) {
    return (int)(1 << level) - 1;
}

get_offset_from_first(int index) {
    return (int)(index - ((1 << get_level_from_index(index)) - 1));
}

void buddyAllocator_init(buddy_allocator* b_alloc, int min_size, int n_levels, char* mem, uint8_t *buf) {
    b_alloc -> n_levels = n_levels;
    b_alloc -> min_size = min_size;
    b_alloc -> mem = mem;
    
    //inizializzazione del bitmap
    int n_bits = (1 << n_levels) - 1; //numero di bit necessari per rappresentare un albero binario completo con n_levels. -> assegno un bit ad ogni nodo
    BitMap_init(&b_alloc->bitmap, n_bits, buf);

    //pongo lo stato dei bitmap a 0 (free)
    for (int i = 0; i < n_bits, i++) {
        BitMap_setBit(&b_alloc->bitmap, i, 0);
    }
    printf("**** buddy allocator inizialization finished **** \n");
}

int buddyAllocator_free_block_at_level(buddy_allocator * b_alloc, int level) {
    assert(level < b_alloc->n_levels);

    int start_idx = get_first_idx_at_level(level);
    int end_idx = get_first_idx_at_level(level + 1);

    for (int i = start_idx; i < end_idx; i++) {
        //controllo che il blocco sia libero
        if (BitMap_bit(&b_alloc->bitmap, i) == 1) {
            return i; //blocco libero trovato
        }
    }
    return -1; //nessun blocco libero
}

int buddyAllocator_find_min_level(buddy_allocator * b_alloc, int size) {
    int size_curr = b_alloc->min_size; //inizilizzo con la dimensione minima di blocco del buddy
    int level = b_alloc->n_levels -1; //livello massimo dell'albero (-1 perchè si parte da 0)
    while (size_curr < size) {
        level--;
        size_curr = size_curr * 2; //raddoppio la dim in quanto il buddy allocator gestisce blocchi di dimensioni di potenze di 2
    }
    return level;
}

int buddyAllocator_find_free_block(buddy_allocator * b_alloc, int level) {
    if (level < 0) {
        printf("Buddy da allocare non trovato per questa richiesta");
        return -1;
    }
    assert(level <= b_alloc->n_levels);

    int b_index = buddyAllocator_free_block_at_level(b_alloc, level);

    //blocco libero trovato al livello corrente
    if (b_index != -1) {
        printf("buddy trovato al livello %d: \n", level);
        BitMap_setBit(&b_alloc->bitmap, b_index, 0); //imposto il blocco occupato nella bitmap
        return b_index; 
    }

    //cerco un buddy libero nel livello precedente
    int p_index = buddyAllocator_find_free_block(b_alloc, level-1);

    //non ci sono buddy disponibili
    if (p_index == -1) 
        return -1;

    //il parent buddy è disponibile, faccio lo split ponendo il destro libero e restituisco il sinistro
    BitMap_setBit(&b_alloc->bitmap, get_right_child_idx(p_index), 1);
    return get_left_child_idx(p_index);
}

int * buddyAllocator_get_address(buddy_allocator * b_alloc, int index, int level) {
    
    int offset = get_offset_from_first(index); //calcolo l'offset (posizione del blocco nel livello)
    int dim_block = (1 << (b_alloc->n_levels - level - 1)) * b_
    
    alloc->min_bucket_size; //calcolo dim blocco dato il livello

    int pos = offset * dim_block; //indice di posizione assoluta del blocco nell'intera area di memoria

    int * addr = (int*)(b_alloc->mem + pos); //indirizzo risultante è un puntatore all'inizio del blocco di memoria

    return addr;
}

void * buddyAllocator_alloc(buddy_allocator * b_alloc, int size) {
    int level = buddyAllocator_find_min_level(b_alloc, size+sizeof(int)); //livello minimo da cui iniziare a cercare un blocco libero

    if(level == -1){
    printf("livello con blocco abbastanza grande da soddisfare la richiesta non trovato\n");
    perror("errore buddyAllocator_alloc: no memoria disponibile");
    return NULL;
    }

    int block_index = buddyAllocator_find_free_block(b_alloc, level); //cerco il blocco libero a partire dal livello minimo trovato

    //non ho trovato alcun blocco libero
    if (block_index == -1) {
        perror("errore buddyAllocator_alloc: no memoria disponibile");
        return NULL;
    }

    //ho trovato un blocco libero, lo devo allocare
    else {

        int * addr = buddyAllocator_get_address(b_alloc, block_index, level);

        *addr = block_index; //salvo indice del blocco di mem allocata

        return (void*)(addr + 1);
    }

}

void buddyAllocator_free(buddy_allocator * b_alloc, void *ptr) {
    if (ptr == NULL) {
        //gestione nel caso in cui il puntatore è null
        printf("Tentativo di liberare un puntatore NULL \n");
        return NULL;
    }

    //calcolo l'indice del blocco dal puntatore fornito
    int *addr = (int*) ptr;
    int block_index = *(addr-1);

    //controllo se l'indice del blocco è valido
    if (block_index < 0 || block_index >= (1 << b_alloc->n_levels) - 1) {
        perror("tentativo di liberare un blocco con indice non valido");
        return NULL;
    }

    //imposto il bit corrispondente nella bitmap come libero (1)
    BitMap_setBit(b_alloc->bitmap, block_index, 1);

    //se possibile si esegue il merge dei buddy

    int level = get_level_from_index(block_index);

    while (level < b_alloc->n_levels - 1) {

        int buddy_index = buddyAllocator_get_buddy_idx(block_index);
        int parent_index;

        if (buddy_index == -1 || BitMap_bit(&b_alloc->bitmap, buddy_index) == 0) {
            // Non c'è un buddy libero o il buddy non è libero, interrompi la fusione
            break;
        }
        if (block_index < buddy_index) {
            parent_index = get_parent_idx(block_index);
        }
        else {
            parent_index = get_parent_idx(buddy_index);
        }

        //imposto il bit corrispondente come libero
        BitMap_setBit(&b_alloc->bitmap, block_index, 1);
        BitMap_setBit(&b_alloc->bitmap, block_index, 0);
        BitMap_setBit(&b_alloc->bitmap, buddy_index, 0);

        level++;
    }
}
