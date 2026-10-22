#include "types.h"

Arena_LIFO *init_lifo_arena(void *memory, size_t size) {
    if (size < sizeof(Arena_LIFO) || !memory) {
        return NULL;
    }

    Arena_LIFO *arena = memory;

    arena->end_of_arena = (size_t)arena + size;
    arena->alloc_ptr = (size_t)arena->data;

    return arena;
}
