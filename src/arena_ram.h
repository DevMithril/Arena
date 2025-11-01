#ifndef _ARENA_RAM_LIB_H
#define _ARENA_RAM_LIB_H
#include <stdlib.h>

/**
 * Une arena avec un allocateur RAM
 */
typedef struct Arena
{
    size_t _capacity;
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

#endif