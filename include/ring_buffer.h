#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "arena.h"

typedef enum {
    RING_BUFFER_OK = 0,
    RING_BUFFER_ERR_FULL,
    RING_BUFFER_ERR_EMPTY,
    RING_BUFFER_ERR_INVALID,
    RING_BUFFER_ERR_INVALID_CAPACITY
} RingBufferStatus;

typedef enum {
    RING_BUFFER_MODE_STRICT,
    RING_BUFFER_MODE_OVERWRITE
} RingBufferMode;

typedef struct {
    uint32_t event_id;
    uint32_t payload_size;
    uint64_t timestamp;
    char payload[64];
} RingBufferItem;

typedef struct {
    RingBufferItem *buffer;
    size_t capacity;
    size_t mask;
    size_t head;
    size_t tail;
    RingBufferMode mode;
} RingBuffer;

RingBufferStatus ring_buffer_init(RingBuffer *rb, RingBufferItem *buffer_mem, size_t capacity, RingBufferMode mode);
RingBufferStatus ring_buffer_init_from_arena(RingBuffer *rb, Arena *arena, size_t capacity, RingBufferMode mode);
RingBufferStatus ring_buffer_push(RingBuffer *rb, const RingBufferItem *item);
RingBufferStatus ring_buffer_pop(RingBuffer *rb, RingBufferItem *out_item);
RingBufferStatus ring_buffer_peek(const RingBuffer *rb, RingBufferItem *out_item);
bool ring_buffer_is_full(const RingBuffer *rb);
bool ring_buffer_is_empty(const RingBuffer *rb);
size_t ring_buffer_size(const RingBuffer *rb);
void ring_buffer_clear(RingBuffer *rb);

#endif
