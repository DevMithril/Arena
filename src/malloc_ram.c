#include "types.h"

void *malloc_ram(Arena *arena, size_t size)
{
    Chunk *chunk = arena->free_chunks;
    Chunk *prev = chunk;

    while (chunk && chunk->capacity < size)
    {
        prev = chunk;
        chunk = chunk->next_free;
    }
    
    if (!chunk) return NULL;

    Chunk *next;

    if (chunk->capacity < size + sizeof(Chunk))
    {
        next = chunk->next_free;
    }
    else
    {
        next = (Chunk*)(chunk->data + size);
        next->next_free = chunk->next_free;
        next->capacity = chunk->capacity - (sizeof(Chunk) + size);
        chunk->capacity = size;
    }
    
    if (prev == chunk)
    {
        arena->free_chunks = next;
    }
    else
    {
        prev->next_free = next;
    }
    
    chunk->size = size;

    return chunk + 1;
}