#include "types.h"

void *malloc_ram(Arena *arena, size_t size) {
    Chunk *prev = (Chunk*)&arena->free_chunks;
    Chunk *chunk = prev->to.next_free;

    while (chunk && chunk->capacity < size) {
        prev = chunk;
        chunk = chunk->to.next_free;
    }
    
    if (!chunk) {
        return NULL;
    }

    Chunk *next;

    if (chunk->capacity < size + sizeof(Chunk)) {
        next = chunk->to.next_free;
    } else {
        next = (Chunk*)(chunk->data + size);
        next->to.next_free = chunk->to.next_free;
        next->capacity = chunk->capacity - (sizeof(Chunk) + size);
        chunk->capacity = size;
    }
    
    prev->to.next_free = next;
    chunk->to.size = size;

    return chunk + 1;
}
