#include "types.h"
#include "arena_ram.h"

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
    if (!ptr) return malloc_ram(arena, size);
    size_t prev = arena->free_chunks;
    size_t chunk = (size_t)ptr - _METADATA_SIZE;
    size_t next = chunk + _METADATA_SIZE + chunk_cap(chunk);

    while (prev && chunk_nfree(prev) && chunk > chunk_nfree(prev))
    {
        prev = chunk_nfree(prev);
    }
    
    // on phagocyte le chunk suivant si il est libre
    if (next == prev)
    {
        arena->free_chunks = chunk_nfree(next);
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
            arena->free_chunks = new_free;
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