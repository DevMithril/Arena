#ifndef _TYPES_H
#define _TYPES_H
#include <stddef.h>

typedef unsigned char byte_t;

typedef struct Chunk {
    size_t capacity;
    union {
        struct Chunk *next_free;
        size_t size;
    };
    byte_t data[];
} Chunk;

// First_Chunk n'existe que pour empêcher un warning du compilateur
typedef struct First_Chunk {
    size_t capacity;
    Chunk *next_free;
} First_Chunk;

typedef struct Arena {
    First_Chunk free_chunks;
    byte_t data[];
} Arena;

typedef struct Arena_LIFO {
    size_t end_of_arena;
    size_t alloc_ptr;
    byte_t data[];
} Arena_LIFO;

typedef struct Arena_FSC {
    size_t end_of_arena;
    size_t element_size;
    size_t alloc_ptr;
    byte_t data[];
} Arena_FSC;

#endif
