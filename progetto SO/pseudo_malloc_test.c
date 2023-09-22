#include "bit_map.h"
#include "buddy_allocator.h"
#include "pseudo_malloc.h"
#include <stdio.h>

void pseudo_malloc_test() {

    buddy_allocator b_alloc;
    pseudoMalloc_init(&b_alloc);

    //test allocazione e deallocazione pseudo malloc con buddyAllocator_alloc e mmap
    printf("TEST 1: allocazione 512+4 bytes\n");
    void* ptr1 = (char*) pseudoMalloc_alloc(&b_alloc, 512); //512+4 minore di 4096/4 bytes
    if(ptr1!=NULL){
        printf("allocazione blocco da 512 riuscita all'indirizzo %p\n\n", ptr1);
    }
    printf("TEST 2: allocazione 1020+4 bytes\n");
    void* ptr2 = (char*) pseudoMalloc_alloc(&b_alloc, 1020); //1020+4 uguale a 4096/4 bytes
    if(ptr2!=NULL){
        printf("allocazione blocco da 1020 riuscita all'indirizzo %p\n\n", ptr2);
    }
    printf("TEST 3: allocazione 1024+4 bytes\n");
    void* ptr3 = (char*) pseudoMalloc_alloc(&b_alloc, 1024); //1024+4 maggiore a 4096/4 bytes
    if(ptr3!=NULL){
        printf("allocazione blocco da 1024 riuscita all'indirizzo %p\n\n", ptr3);
    }
    printf("TEST 4: allocazione 2048+4 bytes\n");
    char* ptr4 = (char*) pseudoMalloc_alloc(&b_alloc, 2048); //2048+4 maggiore di 4096/4 bytes
    if(ptr4!=NULL){
        printf("allocazione blocco da 2048 riuscita all'indirizzo %p\n\n", ptr4);
    }
    printf("TEST 5: allocazione 4096+4 bytes\n");
    void* ptr5 = (char*) pseudoMalloc_alloc(&b_alloc, 4096); 
    if(ptr5!=NULL){
        printf("allocazione blocco da 4096 riuscita all'indirizzo %p\n\n", ptr5);
    }

    printf("TEST 6: allocazione 0+4 bytes\n");
    void* ptr6 = (char*) pseudoMalloc_alloc(&b_alloc, 0);
    if(ptr6!=NULL){
        printf("allocazione blocco da 0 riuscita all'indirizzo %p\n", ptr6);
    }
    
    printf("TEST 7: allocazione -1+4 bytes\n");
    void* ptr7 = (char*) pseudoMalloc_alloc(&b_alloc, -1);
    if(ptr7!=NULL){
        printf("Allocated -1 bytes at address %p\n", ptr7);
    }

    pseudoMalloc_free(&b_alloc, ptr1, 512);
    printf("deallocazione blocco da 512 riuscita \n\n");
    pseudoMalloc_free(&b_alloc, ptr2, 1020);
    printf("deallocazione blocco da 1020 riuscita \n\n");
    pseudoMalloc_free(&b_alloc, ptr3, 1024);
    printf("deallocazione blocco da 1024 riuscita \n\n");
    pseudoMalloc_free(&b_alloc, ptr4, 2048);
    printf("deallocazione blocco da 2048 riuscita \n\n");
    pseudoMalloc_free(&b_alloc, ptr5, 4096);
    printf("deallocazione blocco da 4096 riuscita \n\n");
    pseudoMalloc_free(&b_alloc, ptr6, 0);
    printf("deallocazione blocco da 0 riuscita \n\n");
    pseudoMalloc_free(&b_alloc, ptr7, -1);
    printf("deallocazione blocco da -1 riuscita \n\n");

    printf("All memory as been freed\n");
    

    //test se supera il limite di memoria 

}

int main() {
    pseudo_malloc_test();
    return 1;
}
