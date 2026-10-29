#include "types.h"

/**
 * Combine un chunk avec le chunk suivant si celui-ci est libre
 * \param chunk l'adresse du chunk
 */
void combine_free_chunks(Chunk *chunk) {
    Chunk *next = (Chunk*)(chunk->data + chunk->capacity);

    if (next == chunk->to.next_free) {
        chunk->to.next_free = next->to.next_free;
        chunk->capacity += sizeof(Chunk) + next->capacity;
    }
}

void free_ram(Arena *arena, void *ptr) {
    if (ptr) {
        Chunk *chunk = (Chunk*)ptr - 1;
        Chunk *prev = (Chunk*)&arena->free_chunks;
        
        while (prev->to.next_free && (size_t)chunk > (size_t)prev->to.next_free) {
            prev = prev->to.next_free;
        }

        chunk->to.next_free = prev->to.next_free;
        prev->to.next_free = chunk;
        combine_free_chunks(chunk);
        combine_free_chunks(prev);
    }
}
