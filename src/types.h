#ifndef _TYPES_H
#define _TYPES_H
#include <stddef.h>

typedef unsigned char byte_t;

typedef struct Arena
{
    size_t capacity;
    size_t free_chunks;
    byte_t data[];
}Arena;

typedef struct Arena_LIFO
{
    size_t end_of_arena;
    size_t alloc_ptr;
    byte_t data[];
}Arena_LIFO;

typedef struct Arena_FSC
{
    size_t end_of_arena;
    size_t element_size;
    size_t alloc_ptr;
    byte_t data[];
}Arena_FSC;

#endif