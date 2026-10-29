#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../arena.h"
#define ARENA_SIZE 4096 *30
#define DATA_COUNT 300 *30

#define test(...)                                   \
    start = clock();                                \
    for (int i = 0; i < DATA_COUNT; i++) {          \
        __VA_ARGS__;                                \
    }                                               \
    exec_time = clock() - start

int main(void) {
    Arena_LIFO *stack = init_lifo_arena(malloc(ARENA_SIZE), ARENA_SIZE);
    Arena *heap = init_ram_arena(malloc(ARENA_SIZE), ARENA_SIZE);
    Arena_FSC *fsc_heap = init_fsc_arena(malloc(ARENA_SIZE), ARENA_SIZE, sizeof(int));

    int *stack_ptr[DATA_COUNT];
    int *heap_ptr[DATA_COUNT];
    int *fsc_heap_ptr[DATA_COUNT];
    int *std_heap_ptr[DATA_COUNT];

    time_t exec_time;
    time_t start;

    printf("Fonction\tTemps d'execution\n");

    test(stack_ptr[i] = malloc_lifo(stack, sizeof(int)));
    printf("malloc_lifo()\t%ld\n", exec_time);

    test(fsc_heap_ptr[i] = malloc_fsc(fsc_heap, sizeof(int)));
    printf("malloc_fsc()\t%ld\n", exec_time);

    test(heap_ptr[i] = malloc_ram(heap, sizeof(int)));
    printf("malloc_ram()\t%ld\n", exec_time);
    
    test(std_heap_ptr[i] = malloc(sizeof(int)));
    printf("malloc()\t%ld\n", exec_time);

    test(free_lifo(stack, stack_ptr[DATA_COUNT - (i+1)]));
    printf("free_lifo()\t%ld\n", exec_time);

    test(free_fsc(fsc_heap, fsc_heap_ptr[i]));
    printf("free_fsc()\t%ld\n", exec_time);

    test(free_ram(heap, heap_ptr[i]));
    printf("free_ram()\t%ld\n", exec_time);
    
    test(free(std_heap_ptr[i]));
    printf("free()\t\t%ld\n", exec_time);

    free(stack);
    free(heap);
    free(fsc_heap);

    return EXIT_SUCCESS;
}
