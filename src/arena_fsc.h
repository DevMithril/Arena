#ifndef _ARENA_FSC_LIB_H
#define _ARENA_FSC_LIB_H
#include <stdlib.h>

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
 * Alloue un bloc mémoire initialisé à 0 dans `arena`
 * \param arena arena où le bloc mémoire sera alloué
 * \note Renvoie NULL en cas d'échec
 */
void *calloc_fsc(Arena_FSC *arena);

/**
 * Libère un bloc mémoire précédemment alloué dans `arena`
 * \param arena arena contenant le bloc mémoire
 * \param ptr adresse mémoire du bloc
 */
void free_fsc(Arena_FSC *arena, void *ptr);

/**
 * Alloue une Arena FSC
 * \param element_size taille d'un élément (en octets)
 * \param max_element_count nombre maximal d'éléments pouvant être stockés dans l'arena
 * \note Renvoie NULL en cas d'échec
 */
Arena_FSC *create_fsc_arena(size_t element_size, size_t max_element_count);

/**
 * Libère la mémoire d'une Arena FSC
 */
void destroy_fsc_arena(Arena_FSC *arena);

#endif