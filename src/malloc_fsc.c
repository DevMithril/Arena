#include "types.h"

void *malloc_fsc(Arena_FSC *arena)
{
    size_t chunk = arena->alloc_ptr;
    size_t elem_size = arena->element_size;
    
    while (*((byte_t*)chunk))
    {
        chunk += elem_size;
    }
    arena->alloc_ptr = chunk;
    
    if (chunk != arena->end_of_arena)
    {
        *((byte_t*)chunk) = 1;
        return (void*)(chunk + 1);
    }
    return NULL;
}