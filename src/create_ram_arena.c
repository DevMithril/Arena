#include "types.h"

Arena *create_ram_arena(size_t capacity, void *alloc(size_t))
{
    if (capacity < sizeof(Chunk)) return NULL;

    Arena *arena = alloc(capacity + sizeof(Arena));
    if (!arena) return NULL;
    
    Chunk *chunk = (Chunk*)arena->data;

    arena->capacity = capacity;
    arena->free_chunks = chunk;
    chunk->capacity = capacity - sizeof(Chunk);
    chunk->next_free = NULL;
    
    return arena;
}