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
    void *ptr = (void*)(arena->_alloc_ptr + origin(arena));
    if (arena->_alloc_ptr + size > arena->_capacity)
    {
        return NULL;
    }

    size_t i;
    for (i = 0; i + sizeof(size_t) < size; i += sizeof(size_t))
    {
        *((size_t*)(ptr) + i) = 0;
    }
    for (; i < size; i++)
    {
        *((unsigned char*)(ptr) + i) = 0;
    }
    
    arena->_alloc_ptr += size;
    return ptr;
}

void free_lifo(Arena_LIFO *arena, void *ptr)
{
    if (((size_t)ptr) < origin(arena))
    {
        return; // pointer invalide
    }
    size_t new_alloc_ptr = ((size_t)ptr) - origin(arena);
    if (new_alloc_ptr <= arena->_alloc_ptr)
    {
        arena->_alloc_ptr = new_alloc_ptr;
    }
}

Arena_LIFO *create_lifo_arena(size_t capacity)
{
    Arena_LIFO *arena = malloc(capacity + sizeof(Arena_LIFO));
    if (!arena)
    {
        return NULL;
    }
    arena->_capacity = capacity;
    arena->_alloc_ptr = 0;
    return arena;
}

void destroy_lifo_arena(Arena_LIFO *arena)
{
    free(arena);
}