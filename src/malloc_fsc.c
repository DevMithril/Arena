#include "../arena.h"
#include "byte_t.h"

void *malloc_fsc(Arena_FSC *arena)
{
    size_t chunk = arena->_alloc_ptr;
    size_t end_of_arena = arena->_capacity + (size_t)arena->_data;
    size_t elem_size = arena->_element_size + 1;
    
    while (*((byte_t*)chunk) && chunk < end_of_arena)
    {
        chunk += elem_size;
    }
    arena->_alloc_ptr = chunk;
    if (chunk == end_of_arena) return NULL;
    
    *((byte_t*)chunk) = 1;
    return (void*)(chunk + 1);
}