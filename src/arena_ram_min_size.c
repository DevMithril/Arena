#include "types.h"

size_t arena_ram_min_size() {
    return sizeof(Chunk) + sizeof(Arena);
}
