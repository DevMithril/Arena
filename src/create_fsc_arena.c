#include "../arena.h"

Arena_FSC *create_fsc_arena(size_t element_size,
                            size_t max_element_count,
                            void *allocator(size_t))
{
    size_t size = (1 + element_size) * max_element_count;
    Arena_FSC *arena = allocator(size + sizeof(Arena_FSC));
    if (!arena) return NULL;
    
    arena->_capacity = size;
    arena->_element_size = element_size;
    arena->_alloc_ptr = (size_t)arena->_data;
    return arena;
}