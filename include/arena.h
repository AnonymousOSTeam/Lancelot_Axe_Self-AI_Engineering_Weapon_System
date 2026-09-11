#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t *buffer;
    size_t capacity;
    size_t offset;
} Arena;

void arena_init(Arena *arena, uint8_t *buffer, size_t capacity);
void *arena_alloc(Arena *arena, size_t size);
void arena_reset(Arena *arena);

#endif // ARENA_H
