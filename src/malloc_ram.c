#include "types.h"

void *malloc_ram(Arena *arena, size_t size)
{
    Chunk *prev = (Chunk*)&arena->free_chunks;
    Chunk *chunk = prev->next_free;

    while (chunk && chunk->capacity < size) {
        prev = chunk;
        chunk = chunk->next_free;
    }
    
    if (!chunk) {
        return NULL;
    }

    Chunk *next;

    if (chunk->capacity < size + sizeof(Chunk)) {
        next = chunk->next_free;
    } else {
        next = (Chunk*)(chunk->data + size);
        next->next_free = chunk->next_free;
        next->capacity = chunk->capacity - (sizeof(Chunk) + size);
        chunk->capacity = size;
    }
    
    prev->next_free = next;
    chunk->size = size;

    return chunk + 1;
}
