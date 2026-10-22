#include "types.h"

void reset_lifo_arena(Arena_LIFO *arena) {
    arena->alloc_ptr = (size_t)arena->data;
}
