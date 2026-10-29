#include "types.h"

void *malloc_fsc(Arena_FSC *arena, size_t size) {
    byte_t *chunk = (byte_t*)arena->alloc_ptr;
    byte_t *alloc_chunk = chunk;
    size_t elem_size = arena->element_size;
    size_t target_count = size / elem_size + ((size % elem_size) ? 1 : 0);
    size_t count = 0;

    while (count != target_count && (size_t)chunk < arena->data_ptr) {
        if (*chunk) {
            count = 0;
        } else {
            if (!count) {
                alloc_chunk = chunk;
            }
            count++;
        }
        chunk++;
    }

    if (count && count == target_count) {
        for (count = 0; count < target_count - 1; count++) {
            *(alloc_chunk + count) = CHAINED_CHUNK;
        }
        *(alloc_chunk + count) = USED_CHUNK;
        if ((size_t)alloc_chunk == arena->alloc_ptr) {
            arena->alloc_ptr = (size_t)alloc_chunk + count + 1;
        }
        return (byte_t*)arena->data_ptr + (alloc_chunk - arena->metadata) * elem_size;
    }

    return NULL;
}
