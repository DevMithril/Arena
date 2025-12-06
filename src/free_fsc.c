#include "../arena.h"
#include "byte_t.h"

void free_fsc(Arena_FSC *arena, void *ptr)
{
    if (ptr)
    {
        *((byte_t*)ptr - 1) = 0;
        if (arena->_alloc_ptr > (size_t)ptr - 1)
        {
            arena->_alloc_ptr = (size_t)ptr - 1;
        }
    }
}