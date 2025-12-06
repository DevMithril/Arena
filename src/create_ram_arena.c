#include "../arena.h"
#include "types.h"
#include "arena_ram.h"

Arena *create_ram_arena(size_t capacity, void *alloc(size_t))
{
    if (capacity < _METADATA_SIZE) return NULL;

    Arena *arena = alloc(capacity + sizeof(Arena));
    if (!arena) return NULL;
    
    size_t chunk = origin(arena);

    arena->capacity = capacity;
    arena->free_chunks = chunk;
    chunk_cap(chunk) = capacity - _METADATA_SIZE;
    chunk_nfree(chunk) = 0;
    
    return arena;
}