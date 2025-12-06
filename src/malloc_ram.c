#include "types.h"

void *malloc_ram(Arena *arena, size_t size)
{
    Chunk *chunk = arena->free_chunks;
    Chunk *prev = chunk;

    while (chunk && chunk->capacity != size &&
           chunk->capacity < size + sizeof(Chunk))
    {
        prev = chunk;
        chunk = chunk->next_free;
    }
    
    if (!chunk) return NULL;

    if (chunk->capacity == size)
    {
        if (prev == chunk)
            {arena->free_chunks = chunk->next_free;}
        else
            {prev->next_free = chunk->next_free;}
        chunk->size = size;
        return chunk + 1;
    }

    Chunk *next = (Chunk*)((size_t)chunk + sizeof(Chunk) + size);

    next->next_free = chunk->next_free;
    
    if (prev == chunk)
        {arena->free_chunks = next;}
    else
        {prev->next_free = next;}
    
    next->capacity = chunk->capacity - (sizeof(Chunk) + size);
    chunk->capacity = size;
    chunk->size = size;

    return chunk + 1;
}