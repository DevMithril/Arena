#include "../arena.h"
#include "types.h"

Arena_LIFO *create_lifo_arena(size_t capacity, void *alloc(size_t))
{
    Arena_LIFO *arena = alloc(capacity + sizeof(Arena_LIFO));
    
    if (arena)
    {
        arena->end_of_arena = (size_t)arena->data + capacity;
        arena->alloc_ptr = (size_t)arena->data;
    }
    return arena;
}