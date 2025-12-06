#ifndef _REALLOC_RAM_H
#define _REALLOC_RAM_H
#include "types.h"

void *malloc_ram(Arena *arena, size_t size);

void free_ram(Arena *arena, void *ptr);

#endif