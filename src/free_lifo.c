#include "types.h"

void free_lifo(Arena_LIFO *arena, void *ptr)
{
    if (ptr && arena->alloc_ptr > (size_t)ptr)
    {
        arena->alloc_ptr = (size_t)ptr;
    }
}