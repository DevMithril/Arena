#include "types.h"

void reset_fsc_arena(Arena_FSC *arena) {
    byte_t *meta = arena->data;
    arena->alloc_ptr = (size_t)meta;
    for (; (size_t)meta < arena->end_of_arena; meta += arena->element_size) {
        if (*meta) *meta = 0;
    }
}
