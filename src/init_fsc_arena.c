#include "types.h"

Arena_FSC *init_fsc_arena(void *memory, size_t size, size_t element_size) {
    if (size < sizeof(Arena_FSC) || !memory) {
        return NULL;
    }

    Arena_FSC *arena = memory;
    size_t data_size = size - sizeof(Arena_FSC);
    data_size -= data_size % element_size;
    element_size += 1;

    arena->end_of_arena = (size_t)arena->data + data_size;
    arena->element_size = element_size;
    arena->alloc_ptr = (size_t)arena->data;
    byte_t *meta = arena->data;
    for (; (size_t)meta < arena->end_of_arena; meta += element_size) {
        if (*meta) *meta = 0;
    }

    return arena;
}
