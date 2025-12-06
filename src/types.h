#ifndef _TYPES_H
#define _TYPES_H
#include <stddef.h>

typedef unsigned char byte_t;

typedef struct Chunk
{
    size_t capacity;
    union
    {
        struct Chunk *next_free;
        size_t size;
    };
    byte_t data[];
}Chunk;

typedef struct Arena
{
    size_t capacity;
    Chunk *free_chunks;
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