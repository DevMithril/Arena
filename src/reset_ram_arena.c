#include "types.h"

void reset_ram_arena(Arena *arena) {
    Chunk *chunk = (Chunk*)arena->data;
    arena->free_chunks = chunk;
    chunk->capacity = arena->capacity - sizeof(Chunk);
    chunk->to.next_free = NULL;
}
