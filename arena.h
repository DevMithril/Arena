/*
================================================================================
    zlib License
================================================================================

Copyright (c) 2026 Théophane Bruyelles

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from
the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented;
   you must not claim that you wrote the original software.
   If you use this software in a product, an acknowledgment in the product
   documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such,
   and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

*/

#ifndef _ARENA_LIB_H
#define _ARENA_LIB_H
#include <stddef.h>

/*
================================================================================
    Arena RAM
================================================================================
*/

/**
 * Une arena avec un allocateur RAM
 */
typedef void Arena;

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
 * Renvoie la taille minimale d'une Arena fonctionnelle
 */
size_t arena_ram_min_size();

/**
 * Initialise une Arena
 * \param memory bloc mémoire dédiée à l'arena
 * \param size taille de `memory` (en octets)
 * \note Renvoie NULL si `size` est inférieur à arena_ram_min_size()
 */
Arena *init_ram_arena(void *memory, size_t size);

/**
 * Réinitialise une Arena
 * \param arena arena à réinitialiser
 */
void reset_ram_arena(Arena *arena);

/*
================================================================================
    Arena LIFO
================================================================================
*/

/*
 * Une arena avec un allocateur LIFO
 */
typedef void Arena_LIFO;

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
 * Renvoie la taille minimale d'une Arena_LIFO fonctionnelle
 */
size_t arena_lifo_min_size();

/**
 * Alloue une Arena_LIFO
 * \param memory bloc mémoire dédiée à l'arena
 * \param size taille de `memory` (en octets)
 * \note Renvoie NULL si `size` est inférieur à arena_lifo_min_size()
 */
Arena_LIFO *init_lifo_arena(void *memory, size_t size);

/**
 * Réinitialise une Arena_LIFO
 * \param arena arena à réinitialiser
 */
void reset_lifo_arena(Arena_LIFO *arena);

/*
================================================================================
    Arena FSC
================================================================================
*/

/**
 * Une arena avec un allocateur FSC (Fixed-Size Chunks)
 */
typedef void Arena_FSC;

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
 * Renvoie la taille minimale d'une Arena_FSC fonctionnelle
 */
size_t arena_fsc_min_size();

/**
 * Alloue une Arena FSC
 * \param memory bloc mémoire dédiée à l'arena
 * \param size taille de `memory` (en octets)
 * \param element_size taille d'un élément de l'arena (en octets)
 * \note Renvoie NULL si `size` est inférieur à arena_fsc_min_size()
 */
Arena_FSC *init_fsc_arena(void *memory, size_t size, size_t element_size);

/**
 * Réinitialise une Arena_FSC
 * \param arena arena à réinitialiser
 */
void reset_fsc_arena(Arena_FSC *arena);

#endif
