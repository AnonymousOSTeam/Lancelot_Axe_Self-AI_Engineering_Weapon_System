#include "arena.h"

void arena_init(Arena *arena, uint8_t *buffer, size_t capacity) {
    arena->buffer = buffer;
    arena->capacity = capacity;
    arena->offset = 0;
}

void *arena_alloc(Arena *arena, size_t size) {
    // Alinhamento de 8 bytes com o operador bitwise AND (&) correto
    size_t aligned_size = (size + 7) & ~((size_t)7);

    if (arena->offset + aligned_size > arena->capacity) {
        return NULL; // Arena esgotada
    }

    void *ptr = &arena->buffer[arena->offset];
    arena->offset += aligned_size;
    return ptr;
}

void arena_reset(Arena *arena) {
    arena->offset = 0;
}
