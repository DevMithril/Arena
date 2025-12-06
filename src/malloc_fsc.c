#include "../arena.h"
#include "byte_t.h"

void *malloc_fsc(Arena_FSC *arena)
{
    size_t chunk = arena->_alloc_ptr;
    size_t elem_size = arena->_element_size;
    
    while (*((byte_t*)chunk))
    {
        chunk += elem_size;
    }
    arena->_alloc_ptr = chunk;
    
    if (chunk != arena->_end_of_arena)
    {
        *((byte_t*)chunk) = 1;
        return (void*)(chunk + 1);
    }
    return NULL;
}