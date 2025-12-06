#include "../arena.h"
#include "types.h"

void free_fsc(Arena_FSC *arena, void *ptr)
{
    if (ptr)
    {
        *((byte_t*)ptr - 1) = 0;
        if (arena->alloc_ptr > (size_t)ptr - 1)
        {
            arena->alloc_ptr = (size_t)ptr - 1;
        }
    }
}