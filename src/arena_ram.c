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

void *realloc_ram(Arena *arena, void *ptr, size_t size)
{
    if (!ptr) return malloc_ram(arena, size);
    size_t prev = arena->_free_chunks;
    size_t chunk = (size_t)ptr - _METADATA_SIZE;
    size_t next = chunk + _METADATA_SIZE + chunk_capacity(chunk);

    while (prev && chunk_next_free(prev) && chunk > chunk_next_free(prev))
    {
        prev = chunk_next_free(prev);
    }
    
    // on phagocyte le chunk suivant si il est libre
    if (next == prev)
    {
        arena->_free_chunks = chunk_next_free(next);
        prev = chunk_next_free(next);
        chunk_capacity(chunk) += _METADATA_SIZE + chunk_capacity(next);
    }
    else if (prev && next == chunk_next_free(prev))
    {
        chunk_next_free(prev) = chunk_next_free(next);
        chunk_capacity(chunk) += _METADATA_SIZE + chunk_capacity(next);
    }
    
    // le chunk est de la taille demandée
    if (chunk_capacity(chunk) == size)
    {
        return ptr;
    }
    
    // le chunk est plus grand que la taille demandée
    if (chunk_capacity(chunk) > size + _METADATA_SIZE)
    {
        size_t new_free = chunk + _METADATA_SIZE + size;
        chunk_capacity(new_free) = chunk_capacity(chunk) - (_METADATA_SIZE + size);
        if (!prev || prev > chunk)
        {
            chunk_next_free(new_free) = prev;
            arena->_free_chunks = new_free;
        }
        else
        {
            chunk_next_free(new_free) = chunk_next_free(prev);
            chunk_next_free(prev) = new_free;
        }
        chunk_capacity(chunk) = size;
        return ptr;
    }

    // pire scenario : le chunk ne peut pas être redimensionné
    void *new_ptr = malloc_ram(arena, size);
    if (new_ptr)
    {
        memcpy_ram(new_ptr, ptr, chunk_capacity(chunk));
    }
    free_ram(arena, ptr);
    return new_ptr;
}

void *calloc_ram(Arena *arena, size_t size)
{
    void *ptr = malloc_ram(arena, size);
    if (!ptr) return NULL;
    return memset_ram(ptr, 0, size);
}

void free_ram(Arena *arena, void *ptr)
{
    if (!ptr) return;
    
    size_t chunk = (size_t)ptr - _METADATA_SIZE;
    size_t prev = arena->_free_chunks;
    if (!prev || prev > chunk)
    {
        arena->_free_chunks = chunk;
        chunk_next_free(chunk) = prev;
        combine_free_chunks(chunk);
        return;
    }
    
    while (chunk_next_free(prev) && chunk > chunk_next_free(prev))
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

void *memcpy_ram(void *dst, void *src, size_t size)
{
    size_t* lsrc;
    size_t* ldst;
    unsigned char* csrc;
    unsigned char* cdst;
    size_t nl = size / sizeof(size_t);
    size_t nc = size % sizeof(size_t);

    for (lsrc = src, ldst = dst; nl; nl--, lsrc++, ldst++)
    {
        *ldst = *lsrc;
    }
    for (csrc = (unsigned char*)lsrc, cdst = (unsigned char*)ldst; nc; nc--, csrc++, cdst++)
    {
        *cdst = *csrc;
    }
    return dst;
}

void *memset_ram(void *ptr, unsigned char value, size_t size)
{
    size_t* l;
    unsigned char* c;
    size_t nl = size / sizeof(size_t);
    size_t nc = size % sizeof(size_t);
    size_t lvalue = value;
    lvalue |= lvalue << 8;
    lvalue |= lvalue << 16;
    lvalue |= lvalue << 32;

    for (l = ptr; nl; nl--, l++)
    {
        *l = lvalue;
    }
    for (c = (unsigned char*)l; nc; nc--, c++)
    {
        *c = value;
    }
    return ptr;
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