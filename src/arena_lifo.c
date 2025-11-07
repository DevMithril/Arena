#include "arena_lifo.h"
/**
 * (size_t) l'adresse du début de la data de l'arena
 */
#define origin(arena) ((size_t)(arena) + sizeof(Arena_LIFO))

void *malloc_lifo(Arena_LIFO *arena, size_t size)
{
    void *ptr = (void*)(arena->_alloc_ptr + origin(arena));
    if (arena->_alloc_ptr + size > arena->_capacity)
    {
        return NULL;
    }
    arena->_alloc_ptr += size;
    return ptr;
}

void *calloc_lifo(Arena_LIFO *arena, size_t size)
{
    void *ptr = malloc_lifo(arena, size);
    if (!ptr) return NULL;

    size_t* l;
    unsigned char* c;
    size_t nl = size / sizeof(size_t);
    size_t nc = size % sizeof(size_t);
    
    for (l = ptr; nl; nl--, l++)
    {
        if (*l) *l = 0;
    }
    for (c = (unsigned char*)l; nc; nc--, c++)
    {
        if (*c) *c = 0;
    }

    return ptr;
}

void free_lifo(Arena_LIFO *arena, void *ptr)
{
    if (!ptr) return;
    size_t new_alloc_ptr = (size_t)ptr - origin(arena);
    if (arena->_alloc_ptr > new_alloc_ptr)
    {
        arena->_alloc_ptr = new_alloc_ptr;
    }
}

Arena_LIFO *create_lifo_arena(size_t capacity)
{
    Arena_LIFO *arena = malloc(capacity + sizeof(Arena_LIFO));
    if (!arena) return NULL;

    arena->_capacity = capacity;
    arena->_alloc_ptr = 0;
    return arena;
}

void destroy_lifo_arena(Arena_LIFO *arena)
{
    free(arena);
}