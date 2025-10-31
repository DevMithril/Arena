#include <stdio.h>
#include <time.h>
#include "arena.h"

#define ARENA_SIZE 4096
#define DATA_COUNT 300

#define test(...)                                   \
    start = clock();                                \
    for (int i = 0; i < DATA_COUNT; i++)            \
    {                                               \
        __VA_ARGS__;                                \
    }                                               \
    exec_time = clock() - start

int main(void)
{
    Arena_LIFO *stack = create_lifo_arena(ARENA_SIZE);
    Arena *heap = create_ram_arena(ARENA_SIZE);
    int *stack_ptr[DATA_COUNT];
    int *heap_ptr[DATA_COUNT];
    int *std_heap_ptr[DATA_COUNT];
    time_t exec_time;
    time_t start;

    printf("Fonction\tTemps d'execution\n");

    test(stack_ptr[i] = malloc_lifo(stack, sizeof(int)));
    printf("malloc_lifo()\t%ld\n", exec_time);

    test(heap_ptr[i] = malloc_ram(heap, sizeof(int)));
    printf("malloc_ram()\t%ld\n", exec_time);
    
    test(std_heap_ptr[i] = malloc(sizeof(int)));
    printf("malloc()\t%ld\n", exec_time);

    test(free_lifo(stack, stack_ptr[DATA_COUNT - (i+1)]));
    printf("free_lifo()\t%ld\n", exec_time);

    test(free_ram(heap, heap_ptr[i]));
    printf("free_ram()\t%ld\n", exec_time);
    
    test(free(std_heap_ptr[i]));
    printf("free()\t\t%ld\n", exec_time);

    destroy_lifo_arena(stack);
    destroy_ram_arena(heap);

    return EXIT_SUCCESS;
}