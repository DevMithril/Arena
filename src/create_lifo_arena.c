#include "../arena.h"

Arena_LIFO *create_lifo_arena(size_t capacity, void *allocator(size_t))
{
    Arena_LIFO *arena = allocator(capacity + sizeof(Arena_LIFO));
    if (!arena) return NULL;

    arena->_end_of_arena = (size_t)arena->_data + capacity;
    arena->_alloc_ptr = (size_t)arena->_data;
    return arena;
}