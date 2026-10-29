#include "types.h"

Arena *init_ram_arena(void *memory, size_t size) {
    if (size < sizeof(Chunk) + sizeof(Arena) || !memory) {
        return NULL;
    }

    Arena *arena = memory;
    Chunk *chunk = (Chunk*)arena->data;

    arena->free_chunks.capacity = size - sizeof(Arena);
    arena->free_chunks.next_free = chunk;
    chunk->capacity = size - (sizeof(Chunk) + sizeof(Arena));
    chunk->to.next_free = NULL;

    return arena;
}
