#include "arena_lifo.h"

void *malloc_lifo(Arena_LIFO *arena, size_t size)
{
    size_t ptr = arena->_alloc_ptr;
    
    if (ptr + size > arena->_end_of_arena)
    {
        return NULL;
    }
    arena->_alloc_ptr += size;

    return (void*)ptr;
}

void *calloc_lifo(Arena_LIFO *arena, size_t size)
{
    void *ptr = malloc_lifo(arena, size);
    if (!ptr) return NULL;

    size_t* l = ptr;
    size_t nc = size % sizeof(size_t);
    
    if (size >= sizeof(size_t))
    {
        size_t nl = size / sizeof(size_t);
        for (; nl; nl--, l++)
        {
            if (*l) *l = 0;
        }
    }
    for (unsigned char *c = (unsigned char*)l; nc; nc--, c++)
    {
        if (*c) *c = 0;
    }
        
    return ptr;
}

void free_lifo(Arena_LIFO *arena, void *ptr)
{
    if (!ptr) return;
    if (arena->_alloc_ptr > (size_t)ptr)
    {
        arena->_alloc_ptr = (size_t)ptr;
    }
}

Arena_LIFO *create_lifo_arena(size_t capacity)
{
    Arena_LIFO *arena = malloc(capacity + sizeof(Arena_LIFO));
    if (!arena) return NULL;

    arena->_end_of_arena = (size_t)arena->_data + capacity;
    arena->_alloc_ptr = (size_t)arena->_data;
    return arena;
}

void destroy_lifo_arena(Arena_LIFO *arena)
{
    free(arena);
}