#ifndef _ARENA_LIFO_LIB_H
#define _ARENA_LIFO_LIB_H
#include <stdlib.h>

/*
 * Une arena avec un allocateur LIFO
 */
typedef struct Arena_LIFO
{
    size_t _end_of_arena;
    size_t _alloc_ptr;
    unsigned char _data[];
}Arena_LIFO;

/**
 * Alloue `size` octets dans `arena`
 * \param arena arena où le bloc mémoire sera alloué
 * \param size nombre d'octets à allouer
 * \note Renvoie NULL en cas d'échec
 */
void *malloc_lifo(Arena_LIFO *arena, size_t size);

/**
 * Alloue `size` octets initialisés à 0 dans `arena`
 * \param arena arena où le bloc mémoire sera alloué
 * \param size nombre d'octets à allouer
 * \note Renvoie NULL en cas d'échec
 */
void *calloc_lifo(Arena_LIFO *arena, size_t size);

/**
 * Libère un bloc mémoire précédemment alloué dans `arena`
 * \param arena arena contenant le bloc mémoire
 * \param ptr adresse mémoire du bloc
 * \note Déalloue toute mémoire ultérieure à `ptr`
 */
void free_lifo(Arena_LIFO *arena, void *ptr);

/**
 * Alloue une Arena_LIFO
 * \param capacity capacité de stockage de l'arena (en octets)
 * \note Renvoie NULL en cas d'échec
 */
Arena_LIFO *create_lifo_arena(size_t capacity);

/**
 * Libère la mémoire d'une Arena_LIFO
 */
void destroy_lifo_arena(Arena_LIFO *arena);

#endif