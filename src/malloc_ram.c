#include "types.h"
#include "arena_ram.h"

void *malloc_ram(Arena *arena, size_t size)
{
    size_t chunk = arena->free_chunks;
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
            {arena->free_chunks = chunk_nfree(chunk);}
        else
            {chunk_nfree(prev) = chunk_nfree(chunk);}
        return (void*)(chunk + _METADATA_SIZE);
    }

    size_t next = chunk + _METADATA_SIZE + size;

    chunk_nfree(next) = chunk_nfree(chunk);
    
    if (prev == chunk)
        {arena->free_chunks = next;}
    else
        {chunk_nfree(prev) = next;}
    
    chunk_cap(next) = chunk_cap(chunk) - (_METADATA_SIZE + size);
    chunk_cap(chunk) = size;

    return (void*)(chunk + _METADATA_SIZE);
}