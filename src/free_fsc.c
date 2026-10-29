#include "types.h"

void free_fsc(Arena_FSC *arena, void *ptr) {
    byte_t *meta;

    if (ptr) {
        meta = arena->metadata + ((size_t)ptr - arena->data_ptr) / arena->element_size;
        if ((size_t)meta < arena->alloc_ptr) {
            arena->alloc_ptr = (size_t)meta;
        }
        while (*meta == CHAINED_CHUNK) {
            *meta = FREE_CHUNK;
            meta++;
        }
        *meta = FREE_CHUNK;
    }
}
