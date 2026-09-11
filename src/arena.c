#include "arena.h"

void arena_init(Arena *arena, uint8_t *buffer, size_t capacity) {
    arena->buffer = buffer;
    arena->capacity = capacity;
    arena->offset = 0;
}

void *arena_alloc(Arena *arena, size_t size) {
    size_t aligned_size = (size + 7) & ~((size_t)7);

    if (arena->offset + aligned_size > arena->capacity) {
        return NULL;
    }

    void *ptr = &arena->buffer[arena->offset];
    arena->offset += aligned_size;
    return ptr;
}

void arena_reset(Arena *arena) {
    arena->offset = 0;
}

size_t arena_available(const Arena *arena) {
    if (arena->offset >= arena->capacity) {
        return 0;
    }
    return arena->capacity - arena->offset;
}

ArenaTemp arena_temp_begin(Arena *arena) {
    ArenaTemp temp;
    temp.arena = arena;
    temp.prev_offset = arena->offset;
    return temp;
}

void arena_temp_end(ArenaTemp temp) {
    if (temp.arena) {
        temp.arena->offset = temp.prev_offset;
    }
}
