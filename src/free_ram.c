#include "types.h"

/**
 * Combine un chunk avec le chunk suivant si celui-ci est libre
 * \param chunk l'adresse du chunk
 */
void combine_free_chunks(Chunk *chunk)
{
    Chunk *next = (Chunk*)((size_t)(chunk + 1) + chunk->capacity);

    if (next != chunk->next_free || !chunk->next_free) return;
    
    chunk->next_free = next->next_free;
    chunk->capacity += sizeof(Chunk) + next->capacity;
}

void free_ram(Arena *arena, void *ptr)
{
    if (!ptr) return;
    
    Chunk *chunk = (Chunk*)((size_t)ptr - sizeof(Chunk));
    Chunk *prev = arena->free_chunks;
    if (!prev || (size_t)prev > (size_t)chunk)
    {
        arena->free_chunks = chunk;
        chunk->next_free = prev;
        combine_free_chunks(chunk);
        return;
    }
    
    while (prev->next_free && (size_t)chunk > (size_t)prev->next_free)
    {
        prev = prev->next_free;
    }
    chunk->next_free = prev->next_free;
    prev->next_free = chunk;

    combine_free_chunks(chunk);
    combine_free_chunks(prev);
}