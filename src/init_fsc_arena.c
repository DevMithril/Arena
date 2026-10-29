#include "types.h"

Arena_FSC *init_fsc_arena(void *memory, size_t size, size_t element_size) {
    if (size < sizeof(Arena_FSC) || !memory) {
        return NULL;
    }

    Arena_FSC *arena = memory;
    size_t element_count = (size - sizeof(Arena_FSC)) / (element_size + 1);
    size_t metadata_size = element_count;

    arena->data_ptr = (size_t)arena->metadata + metadata_size;
    arena->alloc_ptr = (size_t)arena->metadata;
    arena->element_size = element_size;
    byte_t *meta = arena->metadata;
    for (; (size_t)meta < arena->data_ptr; meta += element_size) {
        if (*meta) *meta = FREE_CHUNK;
    }

    return arena;
}
