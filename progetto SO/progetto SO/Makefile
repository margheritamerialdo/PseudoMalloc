CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm -lc

# Nome dell'eseguibile
EXECUTABLE = test

all: $(EXECUTABLE)

$(EXECUTABLE): pseudo_malloc.o buddy_allocator.o bit_map.o pseudo_malloc_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

pseudo_malloc.o: pseudo_malloc.h pseudo_malloc.c
	$(CC) $(CFLAGS) -c -o $@ pseudo_malloc.c

buddy_allocator.o: buddy_allocator.h buddy_allocator.c
	$(CC) $(CFLAGS) -c -o $@ buddy_allocator.c

bit_map.o: bit_map.h bit_map.c
	$(CC) $(CFLAGS) -c -o $@ bit_map.c

pseudo_malloc_test.o: pseudo_malloc.h pseudo_malloc_test.c
	$(CC) $(CFLAGS) -c -o $@ pseudo_malloc_test.c

clean:
	rm -f pseudo_malloc.o buddy_allocator.o bit_map.o pseudo_malloc_test.o $(EXECUTABLE)