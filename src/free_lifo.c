#include "../arena.h"

void free_lifo(Arena_LIFO *arena, void *ptr)
{
    if (!ptr) return;
    if (arena->_alloc_ptr > (size_t)ptr)
    {
        arena->_alloc_ptr = (size_t)ptr;
    }
}