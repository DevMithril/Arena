#include "types.h"
#include "arena_ram.h"

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

void free_ram(Arena *arena, void *ptr)
{
    if (!ptr) return;
    
    size_t chunk = (size_t)ptr - _METADATA_SIZE;
    size_t prev = arena->free_chunks;
    if (!prev || prev > chunk)
    {
        arena->free_chunks = chunk;
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