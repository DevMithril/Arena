#ifndef _ARENA_RAM_H
#define _ARENA_RAM_H
#include "types.h"

/**
 * (size_t) l'adresse du premier chunk de l'arena
 */
#define origin(arena) ((size_t)(arena) + sizeof(Arena))

/**
 * (size_t) capacité de stockage du chunk, `chunk` est un (size_t)
 */
#define chunk_cap(chunk) (*((size_t*)chunk))

/**
 * (size_t) adresse du chunk libre suivant, `chunk` est un (size_t)
 */
#define chunk_nfree(chunk) (*((size_t*)chunk + 1))

/**
 * Taille des metadata d'un chunk (en octets)
 */
#define _METADATA_SIZE (2 * sizeof(size_t))

void *malloc_ram(Arena *arena, size_t size);

void free_ram(Arena *arena, void *ptr);

#endif