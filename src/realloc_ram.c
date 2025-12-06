#include "types.h"

void *malloc_ram(Arena *arena, size_t size);

void free_ram(Arena *arena, void *ptr);

/**
 * Copie `size` octets de `src` sur `dst`
 * \param dst adresse mémoire du bloc où seront écrites les données
 * \param src adresse mémoire du bloc où sont lues les données
 * \param size nombre d'octets à copier
 * \return Adresse mémoire `dst`
 */
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

void *realloc_ram(Arena *arena, void *ptr, size_t size)
{
    if (!ptr)
    {
        return malloc_ram(arena, size);
    }
    Chunk *prev = arena->free_chunks;
    Chunk *chunk = (Chunk*)ptr - 1;
    Chunk *next = (Chunk*)(chunk->data + chunk->capacity);

    while (prev && prev->next_free && (size_t)chunk > (size_t)prev->next_free)
    {
        prev = prev->next_free;
    }
    
    // on phagocyte le chunk suivant si il est libre
    if (next == prev)
    {
        arena->free_chunks = next->next_free;
        prev = next->next_free;
        chunk->capacity += sizeof(Chunk) + next->capacity;
    }
    else if (prev && next == prev->next_free)
    {
        prev->next_free = next->next_free;
        chunk->capacity += sizeof(Chunk) + next->capacity;
    }
    
    // pire scenario : le chunk ne peut pas être redimensionné
    if (chunk->capacity < size)
    {
        void *new_ptr = malloc_ram(arena, size);
        if (new_ptr)
        {
            memcpy_ram(new_ptr, ptr, chunk->capacity);
        }
        free_ram(arena, ptr);
        return new_ptr;
    }
    
    // le chunk peut être découpé
    if (chunk->capacity >= size + sizeof(Chunk))
    {
        Chunk *new_free = (Chunk*)(chunk->data + size);
        new_free->capacity = chunk->capacity - (sizeof(Chunk) + size);
        if (!prev || (size_t)prev > (size_t)chunk)
        {
            new_free->next_free = prev;
            arena->free_chunks = new_free;
        }
        else
        {
            new_free->next_free = prev->next_free;
            prev->next_free = new_free;
        }
        chunk->capacity = size;
    }

    chunk->size = size;
    return ptr;
}