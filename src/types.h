#ifndef _TYPES_H
#define _TYPES_H
#include <stddef.h>

#define FREE_CHUNK 0x00
#define USED_CHUNK 0x01
#define CHAINED_CHUNK 0x03

typedef unsigned char byte_t;

typedef struct Chunk {
    size_t capacity;
    union {
        struct Chunk *next_free;
        size_t size;
    } to;
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
    size_t element_size;
    size_t alloc_ptr;
    size_t data_ptr;
    byte_t metadata[];
} Arena_FSC;

#endif
