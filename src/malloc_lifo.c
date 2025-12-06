#include "../arena.h"

void *malloc_lifo(Arena_LIFO *arena, size_t size)
{
    size_t ptr = arena->_alloc_ptr;
    
    if (ptr + size <= arena->_end_of_arena)
    {
        arena->_alloc_ptr += size;
        return (void*)ptr;
    }
    return NULL;
}