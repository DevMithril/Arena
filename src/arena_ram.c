#include "../arena.h"

/**
 * (size_t) l'adresse du premier chunk de l'arena
 */
#define origin(arena) ((size_t)(arena) + sizeof(Arena))

/**
 * (size_t) capacité de stockage du chunk, `chunk` est un (size_t)
 */
#define chunk_cap(chunk) (*((size_t*)chunk))

/**
 * (size_t) adresse du chunk libre suivant, `chunk` est un (size_t)
 */
#define chunk_nfree(chunk) (*((size_t*)chunk + 1))

/**
 * Taille des metadata d'un chunk (en octets)
 */
#define _METADATA_SIZE (2 * sizeof(size_t))

typedef unsigned char byte_t;

/**
 * Combine un chunk avec le chunk suivant si celui-ci est libre
 * \param chunk l'adresse du chunk
 */
void combine_free_chunks(size_t chunk)
{
    size_t next = chunk + _METADATA_SIZE + chunk_cap(chunk);

    if (next != chunk_nfree(chunk) || chunk_nfree(chunk) == 0) return;
    
    chunk_nfree(chunk) = chunk_nfree(next);
    chunk_cap(chunk) += _METADATA_SIZE + chunk_cap(next);
}

void *malloc_ram(Arena *arena, size_t size)
{
    size_t chunk = arena->_free_chunks;
    size_t prev = chunk;

    while (chunk && chunk_cap(chunk) != size &&
           chunk_cap(chunk) < size + _METADATA_SIZE)
    {
        prev = chunk;
        chunk = chunk_nfree(chunk);
    }
    
    if (!chunk) return NULL;

    if (chunk_cap(chunk) == size)
    {
        if (prev == chunk)
            {arena->_free_chunks = chunk_nfree(chunk);}
        else
            {chunk_nfree(prev) = chunk_nfree(chunk);}
        return (void*)(chunk + _METADATA_SIZE);
    }

    size_t next = chunk + _METADATA_SIZE + size;

    chunk_nfree(next) = chunk_nfree(chunk);
    
    if (prev == chunk)
        {arena->_free_chunks = next;}
    else
        {chunk_nfree(prev) = next;}
    
    chunk_cap(next) = chunk_cap(chunk) - (_METADATA_SIZE + size);
    chunk_cap(chunk) = size;

    return (void*)(chunk + _METADATA_SIZE);
}

void *realloc_ram(Arena *arena, void *ptr, size_t size)
{
    if (!ptr) return malloc_ram(arena, size);
    size_t prev = arena->_free_chunks;
    size_t chunk = (size_t)ptr - _METADATA_SIZE;
    size_t next = chunk + _METADATA_SIZE + chunk_cap(chunk);

    while (prev && chunk_nfree(prev) && chunk > chunk_nfree(prev))
    {
        prev = chunk_nfree(prev);
    }
    
    // on phagocyte le chunk suivant si il est libre
    if (next == prev)
    {
        arena->_free_chunks = chunk_nfree(next);
        prev = chunk_nfree(next);
        chunk_cap(chunk) += _METADATA_SIZE + chunk_cap(next);
    }
    else if (prev && next == chunk_nfree(prev))
    {
        chunk_nfree(prev) = chunk_nfree(next);
        chunk_cap(chunk) += _METADATA_SIZE + chunk_cap(next);
    }
    
    // le chunk est de la taille demandée
    if (chunk_cap(chunk) == size)
    {
        return ptr;
    }
    
    // le chunk est plus grand que la taille demandée
    if (chunk_cap(chunk) > size + _METADATA_SIZE)
    {
        size_t new_free = chunk + _METADATA_SIZE + size;
        chunk_cap(new_free) = chunk_cap(chunk) - (_METADATA_SIZE + size);
        if (!prev || prev > chunk)
        {
            chunk_nfree(new_free) = prev;
            arena->_free_chunks = new_free;
        }
        else
        {
            chunk_nfree(new_free) = chunk_nfree(prev);
            chunk_nfree(prev) = new_free;
        }
        chunk_cap(chunk) = size;
        return ptr;
    }

    // pire scenario : le chunk ne peut pas être redimensionné
    void *new_ptr = malloc_ram(arena, size);
    if (new_ptr)
    {
        memcpy_ram(new_ptr, ptr, chunk_cap(chunk));
    }
    free_ram(arena, ptr);
    return new_ptr;
}

void free_ram(Arena *arena, void *ptr)
{
    if (!ptr) return;
    
    size_t chunk = (size_t)ptr - _METADATA_SIZE;
    size_t prev = arena->_free_chunks;
    if (!prev || prev > chunk)
    {
        arena->_free_chunks = chunk;
        chunk_nfree(chunk) = prev;
        combine_free_chunks(chunk);
        return;
    }
    
    while (chunk_nfree(prev) && chunk > chunk_nfree(prev))
    {
        prev = chunk_nfree(prev);
    }
    chunk_nfree(chunk) = chunk_nfree(prev);
    chunk_nfree(prev) = chunk;

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
    byte_t* csrc;
    byte_t* cdst;
    size_t nl = size / sizeof(size_t);
    size_t nc = size % sizeof(size_t);

    for (lsrc = src, ldst = dst; nl; nl--, lsrc++, ldst++)
    {
        *ldst = *lsrc;
    }
    for (csrc = (byte_t*)lsrc, cdst = (byte_t*)ldst; nc; nc--, csrc++, cdst++)
    {
        *cdst = *csrc;
    }
    return dst;
}

Arena *create_ram_arena(size_t capacity, void *allocator(size_t))
{
    if (capacity < _METADATA_SIZE) return NULL;

    Arena *arena = allocator(capacity + sizeof(Arena));
    if (!arena) return NULL;
    
    size_t chunk = origin(arena);

    arena->_capacity = capacity;
    arena->_free_chunks = chunk;
    chunk_cap(chunk) = capacity - _METADATA_SIZE;
    chunk_nfree(chunk) = 0;
    
    return arena;
}