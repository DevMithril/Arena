#include "types.h"

void *malloc_fsc(Arena_FSC *arena) {
    size_t elem_size = arena->element_size;
    
    while (*((byte_t*)arena->alloc_ptr) && arena->alloc_ptr < arena->end_of_arena) {
        arena->alloc_ptr += elem_size;
    }

    if (arena->alloc_ptr < arena->end_of_arena) {
        *((byte_t*)arena->alloc_ptr) = 1;
        return (void*)(arena->alloc_ptr + 1);
    }
    return NULL;
}
