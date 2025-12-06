#include "../arena.h"

Arena_FSC *create_fsc_arena(size_t size, size_t count, void *alloc(size_t))
{
    size_t total_size = (size + 1) * count;
    Arena_FSC *arena = alloc(total_size + sizeof(Arena_FSC) + 1);
    if (arena)
    {
        arena->_end_of_arena = (size_t)arena->_data + total_size;
        arena->_element_size = size + 1;
        arena->_alloc_ptr = (size_t)arena->_data;
    }
    return arena;
}