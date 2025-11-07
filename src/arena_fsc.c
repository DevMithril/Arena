#include "arena_fsc.h"

/**
 * Écrit 0 sur `size` octets de `ptr`
 * \param ptr adresse mémoire du bloc
 * \param size nombre d'octets à écrire
 * \return Adresse mémoire `ptr`
 */
void *_memset0_fsc(void *ptr, size_t size)
{
    size_t* l = ptr;
    size_t nc = size % sizeof(size_t);

    if (size >= sizeof(size_t))
    {
        size_t nl = size / sizeof(size_t);
        for (; nl; nl--, l++)
        {
            if (*l) *l = 0;
        }
    }
    for (unsigned char *c = (unsigned char*)l; nc; nc--, c++)
    {
        if (*c) *c = 0;
    }

    return ptr;
}

void *malloc_fsc(Arena_FSC *arena)
{
    size_t chunk = arena->_alloc_ptr;
    size_t end_of_arena = arena->_capacity + (size_t)arena->_data;
    size_t elem_size = arena->_element_size + 1;
    
    while (*((unsigned char*)chunk) && chunk < end_of_arena)
    {
        chunk += elem_size;
    }
    arena->_alloc_ptr = chunk;
    if (chunk == end_of_arena) return NULL;
    
    *((unsigned char*)chunk) = 1;
    return (void*)(chunk + 1);
}

void *calloc_fsc(Arena_FSC *arena)
{
    void *ptr = malloc_fsc(arena);
    if (!ptr) return NULL;
    
    _memset0_fsc(ptr, arena->_element_size);
    return ptr;
}

void free_fsc(Arena_FSC *arena, void *ptr)
{
    if (!ptr) return;
    *((unsigned char*)ptr - 1) = 0;
    if (arena->_alloc_ptr > (size_t)ptr - 1)
    {
        arena->_alloc_ptr = (size_t)ptr - 1;
    }
}

Arena_FSC *create_fsc_arena(size_t element_size, size_t max_element_count)
{
    size_t size = (1 + element_size) * max_element_count;
    Arena_FSC *arena = malloc(size + sizeof(Arena_FSC));
    if (!arena) return NULL;

    _memset0_fsc(arena->_data, size);
    
    arena->_capacity = size;
    arena->_element_size = element_size;
    arena->_alloc_ptr = (size_t)arena->_data;
    return arena;
}

void destroy_fsc_arena(Arena_FSC *arena)
{
    free(arena);
}