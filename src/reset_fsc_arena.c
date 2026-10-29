#include "types.h"

void reset_fsc_arena(Arena_FSC *arena) {
    byte_t *meta = arena->metadata;
    arena->alloc_ptr = (size_t)meta;
    for (; (size_t)meta < arena->data_ptr; meta += arena->element_size) {
        if (*meta) *meta = FREE_CHUNK;
    }
}
