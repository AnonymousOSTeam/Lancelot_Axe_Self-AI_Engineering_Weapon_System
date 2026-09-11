#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t *buffer;
    size_t capacity;
    size_t offset;
} Arena;

typedef struct {
    Arena *arena;
    size_t prev_offset;
} ArenaTemp;

// Protótipos das funções da Arena
void arena_init(Arena *arena, uint8_t *buffer, size_t capacity);
void *arena_alloc(Arena *arena, size_t size);
void arena_reset(Arena *arena);

// Funções utilitárias e de Snapshot/Temp
size_t arena_available(const Arena *arena);
ArenaTemp arena_temp_begin(Arena *arena);
void arena_temp_end(ArenaTemp temp);

#endif // ARENA_H
