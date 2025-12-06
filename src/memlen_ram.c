#include "types.h"

size_t memlen_ram(Arena *arena, void *ptr)
{
    if (!ptr || !arena) return 0;
    return *((size_t*)ptr - 2);
}