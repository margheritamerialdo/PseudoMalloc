#include "bit_map.h"
#include "buddy_allocator.h"
#include "pseudo_malloc.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "sys/mman.h"
#include <stdio.h>

#define PAGE_SIZE 4096              //misura delle pagine più comunemente usata
#define MAX_BUDDY_SIZE 1024*1024    //(1MB)
#define B_LEVELS 9                  // visto negli esempi (source) del corso
#define MIN_BUDDY_SIZE (MAX_BUDDY_SIZE >> (B_LEVELS - 1)) //((MAX_BUDDY_SIZE/2^(B_LEVEL-1) = 4KB) valore più piccolo che il buddy allocator può gestire.
#define N_BUDDIES (1 << B_LEVELS) -1                      //numero di buddies nell'albero (2^B_LEVEL) - 1

int write_on_mem(int size, void *ptr) {
    if (size <= 0) {
        printf("dim non valida in scrittura\n");
        return -1; // Restituisci -1 per indicare errore (dimensione non valida)
    }
    size += 4; 
    if (size <= PAGE_SIZE / 4) {
        // Scrivi il valore 1 nel blocco di memoria allocato con buddy
        *(int*)ptr = 1;
        return 0; // Restituisci 1 per indicare successo
    } else if (size > PAGE_SIZE / 4) {
        // Scrivi il valore 10 nel blocco di memoria allocato con mmap
        *(int*)ptr = 10;
        return 0; // Restituisci 1 per indicare successo
    }
}

int read_from_mem(int size, void *ptr) {
    if (size <= 0) {
        printf("dim non valida in lettura\n");
        return -1; // Restituisci 0 per indicare errore (dimensione non valida)
    }
    size += 4; 

    if (size <= PAGE_SIZE / 4) {
        // Leggi il valore dal blocco di memoria e restituiscilo
        return *(int*)ptr;
    } else if (size > PAGE_SIZE / 4) {
        // Leggi il valore dal blocco di memoria e restituiscilo
        return *(int*)ptr;
    }
}

void pseudo_malloc_test() {

    printf("\n **** TEST SULLA PSEUDO MALLOC **** \n\n");

    buddy_allocator b_alloc;
    pseudoMalloc_init(&b_alloc);

    //test allocazione e deallocazione pseudo malloc con buddyAllocator_alloc e mmap
    printf("TEST 1: allocazione 512+4 bytes\n");
    void* ptr1 = (char*) pseudoMalloc_alloc(&b_alloc, 512); //512+4 minore di 4096/4 bytes
    if(ptr1!=NULL){
        printf("allocazione blocco da 512 riuscita all'indirizzo %p\n", ptr1);
        
        write_on_mem(512, ptr1);
        printf("Valore letto dalla memoria: %d \n", read_from_mem(512, ptr1));

        pseudoMalloc_free(&b_alloc, ptr1);
    }
    else {
        printf("problema con l'allocazione blocco da 512 \n\n");
    }

    printf("TEST 2: allocazione 1020+4 bytes\n");
    void* ptr2 = (char*) pseudoMalloc_alloc(&b_alloc, 1020); //1020+4 uguale a 4096/4 bytes
    if(ptr2!=NULL){
        printf("allocazione blocco da 1020 riuscita all'indirizzo %p\n", ptr2);
        
        write_on_mem(1020, ptr2);
        printf("Valore letto dalla memoria: %d \n", read_from_mem(1020, ptr2));

        pseudoMalloc_free(&b_alloc, ptr2);
    }
    else {
        printf("problema con l'allocazione blocco da 1020 \n\n");
    }

    printf("TEST 3: allocazione 1024+4 bytes\n");
    void* ptr3 = (char*) pseudoMalloc_alloc(&b_alloc, 1024); //1024+4 maggiore a 4096/4 bytes
    if(ptr3!=NULL){
        printf("allocazione blocco da 1024 riuscita all'indirizzo %p\n", ptr3);
        
        write_on_mem(1024, ptr3);
        printf("Valore letto dalla memoria: %d \n", read_from_mem(1024, ptr3));

        pseudoMalloc_free(&b_alloc, ptr3);
    }
    else {
        printf("problema con l'allocazione blocco da 1024 \n\n");
    }

    printf("TEST 4: allocazione 2048+4 bytes\n");
    char* ptr4 = (char*) pseudoMalloc_alloc(&b_alloc, 2048); //2048+4 maggiore di 4096/4 bytes
    if(ptr4!=NULL){
        printf("allocazione blocco da 2048 riuscita all'indirizzo %p\n", ptr4);
        
        write_on_mem(2048, ptr4);
        printf("Valore letto dalla memoria: %d \n", read_from_mem(2048, ptr4));

        pseudoMalloc_free(&b_alloc, ptr4);
    }
    else {
        printf("problema con l'allocazione blocco da 2048 \n\n");
    }

    printf("TEST 5: allocazione 4096+4 bytes\n");
    void* ptr5 = (char*) pseudoMalloc_alloc(&b_alloc, 4096); 
    if(ptr5!=NULL){
        printf("allocazione blocco da 4096 riuscita all'indirizzo %p\n", ptr5);
        
        write_on_mem(4096, ptr5);
        printf("Valore letto dalla memoria: %d \n", read_from_mem(4096, ptr5));

        pseudoMalloc_free(&b_alloc, ptr5);
    }
    else {
        printf("problema con l'allocazione blocco da 4096 \n\n");
    }

    printf("TEST 5.1: allocazione 4096+4 bytes\n");
    void* ptr51 = (char*) pseudoMalloc_alloc(&b_alloc, 4096); 
    if(ptr51!=NULL){
        printf("allocazione blocco da 4096 riuscita all'indirizzo %p\n", ptr51);
        
        write_on_mem(0, ptr51);
        printf("Valore letto dalla memoria: %d \n", read_from_mem(0, ptr51));

        pseudoMalloc_free(&b_alloc, ptr51);
    }
    else {
        printf("problema con l'allocazione blocco da 4096 \n\n");
    }

    printf("TEST 6: allocazione 0+4 bytes\n");
    void* ptr6 = (char*) pseudoMalloc_alloc(&b_alloc, 0);
    
    if(ptr6!=NULL){
        printf("allocazione blocco da 0 riuscita all'indirizzo %p\n", ptr6);
        pseudoMalloc_free(&b_alloc, ptr6);
    }
    else {
        printf("problema con l'allocazione blocco da 0, giusto! \n\n");
    }
    
    printf("\nTEST 7: allocazione -1+4 bytes\n");
    void* ptr7 = (char*) pseudoMalloc_alloc(&b_alloc, -1);
    if(ptr7!=NULL){
        printf("allocazione blocco da -1 riuscita all'indirizzo %p\n", ptr7);
        pseudoMalloc_free(&b_alloc, ptr7);
    }
    else {
        printf("problema con l'allocazione blocco da -1, giusto! \n\n");
    }
}

void buddy_allocator_test() {

    printf("\n **** TEST SUL BUDDY ALLOCATOR **** \n\n");

    buddy_allocator b_alloc;
    uint8_t buf[N_BUDDIES];
    char mem[MAX_BUDDY_SIZE];
    
    buddyAllocator_init(&b_alloc, MIN_BUDDY_SIZE, B_LEVELS, mem, buf);
    
    //test per allocazione e deallocazione
    printf("TEST 0: allocazione blocco da 1024 \n");
    void *ptr0 = buddyAllocator_alloc(&b_alloc, 1024);  //troppo piccolo, la dimensione minima è 4096
    if (ptr0 == NULL) {
        printf("problema con l'allocazione blocco da 1024 \n \n");
    }
    else {
        printf("allocazione blocco da 1024 riuscita all'indirizzo: %p \n", ptr0);
        buddyAllocator_free(&b_alloc, ptr0);
      
    }
    printf("\nTEST 1: allocazione blocco da 2048 \n");
    void *ptr1 = buddyAllocator_alloc(&b_alloc, 2048);  //troppo piccolo, la dimensione minima è 4096
    if (ptr1 == NULL) {
        printf("problema con l'allocazione blocco da 2048 \n \n");
    }
    else {
        printf("allocazione blocco da 2048 riuscita all'indirizzo: %p \n", ptr1);
        buddyAllocator_free(&b_alloc, ptr1);
        
    }
    printf("\nTEST 2: allocazione blocco da 4096 \n");
    void *ptr2 = buddyAllocator_alloc(&b_alloc, 4096); 
    if (ptr2 == NULL) {
        printf("problema con l'allocazione blocco da 4096 \n \n");
    }
    else {
        printf("allocazione blocco da 4096 riuscita all'indirizzo: %p \n", ptr2);
        buddyAllocator_free(&b_alloc, ptr2);
       
    }
    printf("\nTEST 3: allocazione blocco da 6144 \n");
    void *ptr3 = buddyAllocator_alloc(&b_alloc, 6144); 
    if (ptr3 == NULL) {
        printf("problema con l'allocazione blocco da 6144 \n \n");
    }
    else {
        printf("allocazione blocco da 6144 riuscita all'indirizzo: %p \n", ptr3);
        buddyAllocator_free(&b_alloc, ptr3);
        
    }
    printf("\nTEST 4: allocazione blocco da 500000 \n");
    int *ptr4 = (int *) buddyAllocator_alloc(&b_alloc, 500000); 
    if (ptr4 == NULL) {
        printf("problema con l'allocazione blocco da 500000 \n \n");
    }
    else {
        printf("allocazione blocco da 500000 riuscita all'indirizzo: %p \n", ptr4);
        buddyAllocator_free(&b_alloc, ptr4);
    
    }
    printf("\nTEST 5: allocazione blocco da MAX_BUDDY_SIZE \n");
    void *ptr5 = buddyAllocator_alloc(&b_alloc, MAX_BUDDY_SIZE); 
    if (ptr5 == NULL) {
        printf("problema con l'allocazione blocco da MAX_BUDDY_SIZE, giusto! dimensione troppo elevata \n \n");
    }
    else {
        printf("allocazione blocco da MAX_BUDDY_SIZE riuscita all'indirizzo: %p \n", ptr5);
        buddyAllocator_free(&b_alloc, ptr5);
    }

    printf("TEST 6: allocazione due blocchi la cui somma supera dim memoria \n");
    void *ptr6 = buddyAllocator_alloc(&b_alloc, 524288); 
    void *ptr7 = buddyAllocator_alloc(&b_alloc, 524288); 
    if (ptr5 != NULL && ptr6 != NULL) {
        printf("problema con allocazione due blocchi la cui somma supera dim memoria \n \n");
        buddyAllocator_free(&b_alloc, ptr6);
        buddyAllocator_free(&b_alloc, ptr7);
    }
    else if (ptr6 != NULL && ptr7 == NULL) {
        printf("allocazione ptr6 riuscita e ptr7 no, GIUSTO! all'indirizzo: %p \n", ptr6);
        buddyAllocator_free(&b_alloc, ptr6);
    }
    else if (ptr6 == NULL && ptr7 == NULL) {
        printf("allocazione ptr6 e ptr7 non riuscita, ERRORE! \n");
    }

    printf("\nTEST 7: allocazione blocco da 1000000 \n");
    void *ptr8 = buddyAllocator_alloc(&b_alloc, 1000000); 
    if (ptr8 == NULL) {
        printf("problema con l'allocazione blocco da 1000000 \n \n");
    }
    else {
        printf("allocazione blocco da 1000000 riuscita all'indirizzo: %p \n", ptr7);
        buddyAllocator_free(&b_alloc, ptr8);
    }

    printf("\n **** FINE TEST SUL BUDDY ALLOCATOR **** \n\n");

}

int main() {
    buddy_allocator_test();
    pseudo_malloc_test();
    return 0;
}