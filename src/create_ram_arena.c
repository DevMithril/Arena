#include "../arena.h"
#include "arena_ram.h"

Arena *create_ram_arena(size_t capacity, void *allocator(size_t))
{
    if (capacity < _METADATA_SIZE) return NULL;

    Arena *arena = allocator(capacity + sizeof(Arena));
    if (!arena) return NULL;
    
    size_t chunk = origin(arena);

    arena->_capacity = capacity;
    arena->_free_chunks = chunk;
    chunk_cap(chunk) = capacity - _METADATA_SIZE;
    chunk_nfree(chunk) = 0;
    
    return arena;
}