#include "../arena.h"

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
    
    arena->_capacity = size;
    arena->_element_size = element_size;
    arena->_alloc_ptr = (size_t)arena->_data;
    return arena;
}

void destroy_fsc_arena(Arena_FSC *arena)
{
    free(arena);
}