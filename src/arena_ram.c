#include "arena_ram.h"

/**
 * (size_t) l'adresse du premier chunk de l'arena
 */
#define origin(arena) ((size_t)(arena) + sizeof(Arena))

/**
 * (size_t) capacité de stockage du chunk, `chunk` est un (size_t)
 */
#define chunk_capacity(chunk) (*((size_t*)chunk))

/**
 * (size_t) adresse du chunk libre suivant, `chunk` est un (size_t)
 */
#define chunk_next_free(chunk) (*((size_t*)chunk + 1))

/**
 * Taille des metadata d'un chunk (en octets)
 */
#define _METADATA_SIZE (2 * sizeof(size_t))

/**
 * Combine un chunk avec le chunk suivant si celui-ci est libre
 * \param chunk l'adresse du chunk
 */
void combine_free_chunks(size_t chunk)
{
    size_t next = chunk + _METADATA_SIZE + chunk_capacity(chunk);

    if (next != chunk_next_free(chunk) || chunk_next_free(chunk) == 0) return;
    
    chunk_next_free(chunk) = chunk_next_free(next);
    chunk_capacity(chunk) += _METADATA_SIZE + chunk_capacity(next);
}

void *malloc_ram(Arena *arena, size_t size)
{
    size_t chunk = arena->_free_chunks;
    size_t prev = chunk;

    while (chunk && chunk_capacity(chunk) != size && chunk_capacity(chunk) < size + _METADATA_SIZE)
    {
        prev = chunk;
        chunk = chunk_next_free(chunk);
    }
    
    if (!chunk) return NULL;

    if (chunk_capacity(chunk) == size)
    {
        if (prev == chunk)
            {arena->_free_chunks = chunk_next_free(chunk);}
        else
            {chunk_next_free(prev) = chunk_next_free(chunk);}
        return (void*)(chunk + _METADATA_SIZE);
    }

    size_t next = chunk + _METADATA_SIZE + size;

    chunk_next_free(next) = chunk_next_free(chunk);
    
    if (prev == chunk)
        {arena->_free_chunks = next;}
    else
        {chunk_next_free(prev) = next;}
    
    chunk_capacity(next) = chunk_capacity(chunk) - (_METADATA_SIZE + size);
    chunk_capacity(chunk) = size;

    return (void*)(chunk + _METADATA_SIZE);
}

void *calloc_ram(Arena *arena, size_t size)
{
    void *ptr = malloc_ram(arena, size);
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

void free_ram(Arena *arena, void *ptr)
{
    if (!ptr) return;
    
    size_t chunk = (size_t)ptr - _METADATA_SIZE;
    size_t prev = arena->_free_chunks;
    if (prev == 0)
    {
        arena->_free_chunks = chunk;
        chunk_next_free(chunk) = 0;
        return;
    }
    if (prev > chunk)
    {
        arena->_free_chunks = chunk;
        chunk_next_free(chunk) = prev;
        combine_free_chunks(chunk);
        return;
    }
    
    while (chunk > chunk_next_free(prev))
    {
        prev = chunk_next_free(prev);
    }
    chunk_next_free(chunk) = chunk_next_free(prev);
    chunk_next_free(prev) = chunk;

    combine_free_chunks(chunk);
    combine_free_chunks(prev);
}

size_t memlen_ram(Arena *arena, void *ptr)
{
    if (!ptr || !arena) return 0;
    return *((size_t*)ptr - 2);
}

Arena *create_ram_arena(size_t capacity)
{
    if (capacity < _METADATA_SIZE) return NULL;

    Arena *arena = malloc(capacity + sizeof(Arena));
    if (!arena) return NULL;
    
    size_t chunk = origin(arena);

    arena->_capacity = capacity;
    arena->_free_chunks = chunk;
    chunk_capacity(chunk) = capacity - _METADATA_SIZE;
    chunk_next_free(chunk) = 0;
    
    return arena;
}

void destroy_ram_arena(Arena *arena)
{
    free(arena);
}