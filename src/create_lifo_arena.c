#include "../arena.h"

Arena_LIFO *create_lifo_arena(size_t capacity, void *alloc(size_t))
{
    Arena_LIFO *arena = alloc(capacity + sizeof(Arena_LIFO));
    
    if (arena)
    {
        arena->_end_of_arena = (size_t)arena->_data + capacity;
        arena->_alloc_ptr = (size_t)arena->_data;
    }
    return arena;
}