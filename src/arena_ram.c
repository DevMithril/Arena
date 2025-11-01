#include "arena_ram.h"

/**
 * (size_t) l'adresse du premier chunk de l'arena
 */
#define origin(arena) ((size_t)(arena) + sizeof(Arena))

/**
 * Taille des metadata d'un chunk (en octets)
 */
#define _METADATA_SIZE (sizeof(unsigned char) + sizeof(size_t))

/**
 * Combine un chunk avec le chunk suivant si celui-ci est libre
 * \param end_arena l'adresse de la fin de l'arena
 * \param chunk l'adresse du chunk
 * \return 1 si une combinaison a été effectuée, 0 sinon
 */
int try_combine_chunks(size_t end_arena, size_t chunk)
{
    size_t *chunk_capacity = (size_t*)(((unsigned char*)(chunk)) + 1);
    size_t next = chunk + _METADATA_SIZE + (*chunk_capacity);
    if (next + _METADATA_SIZE > end_arena)
    {
        return 0;
    }
    unsigned char *next_is_free = (unsigned char*)(next);
    if (!(*next_is_free))
    {
        return 0;
    }
    size_t *next_capacity = (size_t*)(next_is_free + 1);
    *chunk_capacity += _METADATA_SIZE + (*next_capacity);
    return 1;
}

/**
 * Recherche dans `arena` un chunk libre d'au moins `size` octets
 * \param arena arena où la recherche est effectuée
 * \param size capacité voulue pour le chunk (en octets)
 * \return Adresse du chunk libre, ou NULL si aucun n'est trouvé
 * \note Cette fonction combine les chunks libres à la volée
 */
void *get_free_chunk(Arena *arena, size_t size)
{
    size_t current = origin(arena);
    size_t end_arena = current + arena->_capacity;
    unsigned char *current_is_free = (unsigned char*)(current);
    size_t *current_capacity = (size_t*)(current_is_free + 1);

    while (current + _METADATA_SIZE < end_arena)
    {
        if (*current_is_free)
        {
            while (try_combine_chunks(end_arena, current));
            if ((*current_capacity) == size || (*current_capacity) >= size + _METADATA_SIZE)
            {
                return (void*)(current);
            }
        }
        current += _METADATA_SIZE + (*current_capacity);
        current_is_free = (unsigned char*)(current);
        current_capacity = (size_t*)(current_is_free + 1);
    }
    return NULL;
}

void *malloc_ram(Arena *arena, size_t size)
{
    void *chunk = get_free_chunk(arena, size);
    if (!chunk)
    {
        return NULL;
    }

    unsigned char *chunk_is_free = (unsigned char*)(chunk);
    size_t *chunk_capacity = (size_t*)((size_t)(chunk_is_free) + 1);
    
    *chunk_is_free = 0;
    if (*chunk_capacity != size)
    {
        unsigned char *new_is_free = chunk_is_free + size + _METADATA_SIZE;
        size_t *new_capacity = (size_t*)(new_is_free + 1);

        *new_is_free = 1;
        *new_capacity = (*chunk_capacity) - (size + _METADATA_SIZE);
        *chunk_capacity = size;
    }

    return (unsigned char*)(chunk) + _METADATA_SIZE;
}

void *calloc_ram(Arena *arena, size_t size)
{
    void *ptr = malloc_ram(arena, size);
    size_t i;
    for (i = 0; i + sizeof(size_t) < size; i += sizeof(size_t))
    {
        *((size_t*)(ptr) + i) = 0;
    }
    for (; i < size; i++)
    {
        *((unsigned char*)(ptr) + i) = 0;
    }
    return ptr;
}

void free_ram(Arena *arena, void *ptr)
{
    unsigned char *chunk_is_free = (unsigned char*)(ptr) - _METADATA_SIZE;
    *chunk_is_free = 1;
}

size_t memlen_ram(Arena *arena, void *ptr)
{
    return *((size_t*)((size_t)(ptr) - sizeof(size_t)));
}

Arena *create_ram_arena(size_t capacity)
{
    Arena *arena = malloc(capacity + sizeof(Arena));
    if (!arena)
    {
        return NULL;
    }
    
    void *chunk = (void*)origin(arena);
    unsigned char *chunk_is_free = (unsigned char*)(chunk);
    size_t *chunk_capacity = (size_t*)(chunk_is_free + 1);
    
    arena->_capacity = capacity + _METADATA_SIZE;
    *chunk_is_free = 1;
    *chunk_capacity = capacity;
    
    return arena;
}

void destroy_ram_arena(Arena *arena)
{
    free(arena);
}