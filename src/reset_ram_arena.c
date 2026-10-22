#include "types.h"

void reset_ram_arena(Arena *arena) {
    Chunk *chunk = (Chunk*)arena->data;
    arena->free_chunks.next_free = chunk;
    chunk->capacity = arena->free_chunks.capacity - sizeof(Chunk);
    chunk->next_free = NULL;
}
