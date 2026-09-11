#include "ring_buffer.h"
#include <string.h>

static bool is_power_of_two(size_t x) {
    return x && !(x & (x - 1));
}

RingBufferStatus ring_buffer_init(RingBuffer *rb, RingBufferItem *buffer_mem, size_t capacity, RingBufferMode mode) {
    if (!rb || !buffer_mem) return RING_BUFFER_ERR_INVALID;
    if (!is_power_of_two(capacity)) return RING_BUFFER_ERR_INVALID_CAPACITY;

    rb->buffer = buffer_mem;
    rb->capacity = capacity;
    rb->mask = capacity - 1;
    rb->head = 0;
    rb->tail = 0;
    rb->mode = mode;

    return RING_BUFFER_OK;
}

RingBufferStatus ring_buffer_init_from_arena(RingBuffer *rb, Arena *arena, size_t capacity, RingBufferMode mode) {
    if (!rb || !arena) return RING_BUFFER_ERR_INVALID;
    if (!is_power_of_two(capacity)) return RING_BUFFER_ERR_INVALID_CAPACITY;

    RingBufferItem *mem = arena_alloc_align(arena, sizeof(RingBufferItem) * capacity, _Alignof(RingBufferItem));
    if (!mem) return RING_BUFFER_ERR_INVALID;

    return ring_buffer_init(rb, mem, capacity, mode);
}

bool ring_buffer_is_full(const RingBuffer *rb) {
    return rb ? ((rb->head - rb->tail) >= rb->capacity) : false;
}

bool ring_buffer_is_empty(const RingBuffer *rb) {
    return rb ? (rb->head == rb->tail) : true;
}

RingBufferStatus ring_buffer_push(RingBuffer *rb, const RingBufferItem *item) {
    if (!rb || !item) return RING_BUFFER_ERR_INVALID;
    if (rb->capacity == 0) return RING_BUFFER_ERR_INVALID_CAPACITY;

    if (ring_buffer_is_full(rb)) {
        if (rb->mode == RING_BUFFER_MODE_STRICT) {
            return RING_BUFFER_ERR_FULL;
        }
        rb->tail++;
    }

    size_t index = rb->head & rb->mask;
    rb->buffer[index] = *item;
    rb->head++;

    return RING_BUFFER_OK;
}

RingBufferStatus ring_buffer_pop(RingBuffer *rb, RingBufferItem *out_item) {
    if (!rb || !out_item) return RING_BUFFER_ERR_INVALID;
    if (rb->capacity == 0) return RING_BUFFER_ERR_INVALID_CAPACITY;
    if (ring_buffer_is_empty(rb)) return RING_BUFFER_ERR_EMPTY;

    size_t index = rb->tail & rb->mask;
    *out_item = rb->buffer[index];
    rb->tail++;

    return RING_BUFFER_OK;
}

RingBufferStatus ring_buffer_peek(const RingBuffer *rb, RingBufferItem *out_item) {
    if (!rb || !out_item) return RING_BUFFER_ERR_INVALID;
    if (rb->capacity == 0) return RING_BUFFER_ERR_INVALID_CAPACITY;
    if (ring_buffer_is_empty(rb)) return RING_BUFFER_ERR_EMPTY;

    size_t index = rb->tail & rb->mask;
    *out_item = rb->buffer[index];

    return RING_BUFFER_OK;
}

size_t ring_buffer_size(const RingBuffer *rb) {
    return rb ? (rb->head - rb->tail) : 0;
}

void ring_buffer_clear(RingBuffer *rb) {
    if (rb && rb->capacity > 0) {
        rb->head = 0;
        rb->tail = 0;
    }
}
