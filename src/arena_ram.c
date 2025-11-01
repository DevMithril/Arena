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
    size_t *chunk_capacity = (size_t*)(chunk + 1);
    size_t next = chunk + _METADATA_SIZE + (*chunk_capacity);

    if (next + _METADATA_SIZE > end_arena || !(*((unsigned char*)next)))
    {
        return 0;
    }
    
    *chunk_capacity += _METADATA_SIZE + (*((size_t*)(next + 1)));
    return 1;
}

/**
 * Recherche dans `arena` un chunk libre d'au moins `size` octets
 * \param arena arena où la recherche est effectuée
 * \param size capacité voulue pour le chunk (en octets)
 * \return Adresse du chunk libre, ou NULL si aucun n'est trouvé
 */
void *get_free_chunk(Arena *arena, size_t size)
{
    size_t chunk = origin(arena);
    size_t end_arena = chunk + arena->_capacity;
    size_t *chunk_capacity = (size_t*)(chunk + 1);

    while (chunk + _METADATA_SIZE < end_arena)
    {
        if (*((unsigned char*)chunk) && 
            ((*chunk_capacity) == size || (*chunk_capacity) >= size + _METADATA_SIZE))
        {
            return (void*)chunk;
        }
        chunk += _METADATA_SIZE + (*chunk_capacity);
        chunk_capacity = (size_t*)(chunk + 1);
    }
    return NULL;
}

void *malloc_ram(Arena *arena, size_t size)
{
    unsigned char *chunk = get_free_chunk(arena, size);
    if (!chunk) return NULL;

    size_t *chunk_capacity = (size_t*)(chunk + 1);
    
    *chunk = 0;
    if (*chunk_capacity != size)
    {
        unsigned char *new = chunk + size + _METADATA_SIZE;
        size_t *new_capacity = (size_t*)(new + 1);

        *new = 1;
        *new_capacity = (*chunk_capacity) - (size + _METADATA_SIZE);
        *chunk_capacity = size;
    }

    return chunk + _METADATA_SIZE;
}

void *calloc_ram(Arena *arena, size_t size)
{
    void *ptr = malloc_ram(arena, size);
    if (!ptr) return NULL;

    size_t* l;
    unsigned char* c;
    size_t nl = size / sizeof(size_t);
    size_t nc = size % sizeof(size_t);
    for (l = ptr; nl; nl--, l++)
    {
        if (*l) *l = 0;
    }
    for (c = (unsigned char*)l; nc; nc--, c++)
    {
        if (*c) *c = 0;
    }

    return ptr;
}

void free_ram(Arena *arena, void *ptr)
{
    if (!ptr) return;
    
    size_t chunk = (size_t)ptr - _METADATA_SIZE;
    *((unsigned char*)chunk) = 1;
    
    size_t end_arena = origin(arena) + arena->_capacity;
    while (try_combine_chunks(end_arena, chunk));
}

size_t memlen_ram(Arena *arena, void *ptr)
{
    if (!ptr || !arena) return 0;
    return *((size_t*)ptr - 1);
}

Arena *create_ram_arena(size_t capacity)
{
    if (capacity < _METADATA_SIZE) return NULL;

    Arena *arena = malloc(capacity + sizeof(Arena));
    if (!arena) return NULL;
    
    unsigned char *chunk_is_free = (unsigned char*)origin(arena);
    size_t *chunk_capacity = (size_t*)(chunk_is_free + 1);
    
    arena->_capacity = capacity;
    *chunk_is_free = 1;
    *chunk_capacity = capacity - _METADATA_SIZE;
    
    return arena;
}

void destroy_ram_arena(Arena *arena)
{
    free(arena);
}