#ifndef _ARENA_LIB_H
#define _ARENA_LIB_H
#include <stdlib.h>

/*
================================================================================
    Arena RAM
================================================================================
*/

/**
 * Une arena avec un allocateur RAM
 */
typedef struct Arena
{
    size_t _capacity;
    size_t _free_chunks;
    unsigned char _data[];
}Arena;

/**
 * Alloue `size` octets dans `arena`
 * \param arena arena où le bloc mémoire sera alloué
 * \param size nombre d'octets à allouer
 * \note Renvoie NULL en cas d'échec
 */
void *malloc_ram(Arena *arena, size_t size);

/**
 * Re-alloue un bloc précédemment alloué dans `arena`
 * \param arena arena contenant le bloc mémoire
 * \param ptr adresse mémoire du bloc
 * \param size taille du nouveau bloc
 * \note Renvoie NULL en cas d'échec
 */
void *realloc_ram(Arena *arena, void *ptr, size_t size);

/**
 * Libère un bloc mémoire précédemment alloué dans `arena`
 * \param arena arena contenant le bloc mémoire
 * \param ptr adresse mémoire du bloc
 */
void free_ram(Arena *arena, void *ptr);

/**
 * Récupère la taille d'un bloc mémoire alloué
 * \param arena arena où se situe le bloc mémoire
 * \param ptr adresse mémoire du bloc
 * \return Taille du bloc mémoire
 */
size_t memlen_ram(Arena *arena, void *ptr);

/**
 * Copie `size` octets de `src` sur `dst`
 * \param dst adresse mémoire du bloc où seront écrites les données
 * \param src adresse mémoire du bloc où sont lues les données
 * \param size nombre d'octets à copier
 * \return Adresse mémoire `dst`
 */
void *memcpy_ram(void *dst, void *src, size_t size);

/**
 * Alloue une Arena
 * \param capacity capacité de stockage de l'arena (en octets)
 * \note Renvoie NULL en cas d'échec
 * \note `capacity` correspond au stockage reservé pour la data de l'arena
 *                  (i.e. : vos allocations + les metadata associées)
 */
Arena *create_ram_arena(size_t capacity);

/**
 * Libère la mémoire d'une Arena
 */
void destroy_ram_arena(Arena *arena);

/*
================================================================================
    Arena LIFO
================================================================================
*/

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

/*
================================================================================
    Arena FSC
================================================================================
*/

/**
 * Une arena avec un allocateur FSC (Fixed-Size Chunks)
 */
typedef struct Arena_FSC
{
    size_t _capacity;
    size_t _element_size;
    size_t _alloc_ptr;
    unsigned char _data[];
}Arena_FSC;

/**
 * Alloue un bloc mémoire dans `arena`
 * \param arena arena où le bloc mémoire sera alloué
 * \note Renvoie NULL en cas d'échec
 */
void *malloc_fsc(Arena_FSC *arena);

/**
 * Libère un bloc mémoire précédemment alloué dans `arena`
 * \param arena arena contenant le bloc mémoire
 * \param ptr adresse mémoire du bloc
 */
void free_fsc(Arena_FSC *arena, void *ptr);

/**
 * Alloue une Arena FSC
 * \param element_size taille d'un élément (en octets)
 * \param max_element_count nombre maximal d'éléments pouvant être
 *                          stockés dans l'arena
 * \note Renvoie NULL en cas d'échec
 */
Arena_FSC *create_fsc_arena(size_t element_size, size_t max_element_count);

/**
 * Libère la mémoire d'une Arena FSC
 */
void destroy_fsc_arena(Arena_FSC *arena);

#endif