#include "../arena.h"
#include "types.h"

void *malloc_lifo(Arena_LIFO *arena, size_t size)
{
    size_t ptr = arena->alloc_ptr;
    
    if (ptr + size <= arena->end_of_arena)
    {
        arena->alloc_ptr += size;
        return (void*)ptr;
    }
    return NULL;
}