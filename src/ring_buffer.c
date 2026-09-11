#include "ring_buffer.h"
#include <string.h>

RingBufferStatus ring_buffer_init_from_arena(RingBuffer *rb, Arena *arena, size_t capacity, RingBufferMode mode) {
    if (rb == NULL || arena == NULL || capacity == 0) {
        return RING_BUFFER_ERR_INVALID_CAPACITY;
    }

    // Aloca a memória contígua necessária na Arena
    RingBufferItem *mem = arena_alloc(arena, sizeof(RingBufferItem) * capacity);
    if (mem == NULL) {
        return RING_BUFFER_ERR_FULL;
    }

    rb->buffer = mem;
    rb->capacity = capacity;
    rb->head = 0;
    rb->tail = 0;
    rb->mode = mode;

    return RING_BUFFER_OK;
}

bool ring_buffer_is_empty(const RingBuffer *rb) {
    if (rb == NULL) return true;
    return (rb->head == rb->tail);
}

bool ring_buffer_is_full(const RingBuffer *rb) {
    if (rb == NULL) return false;
    return ((rb->tail + 1) % rb->capacity == rb->head);
}

size_t ring_buffer_size(const RingBuffer *rb) {
    if (rb == NULL) return 0;
    if (rb->tail >= rb->head) {
        return rb->tail - rb->head;
    }
    return rb->capacity - (rb->head - rb->tail);
}

RingBufferStatus ring_buffer_push(RingBuffer *rb, const RingBufferItem *item) {
    if (rb == NULL || item == NULL) {
        return RING_BUFFER_ERR_INVALID_CAPACITY;
    }

    if (ring_buffer_is_full(rb)) {
        if (rb->mode == RING_BUFFER_MODE_STRICT) {
            return RING_BUFFER_ERR_FULL;
        } else if (rb->mode == RING_BUFFER_MODE_OVERWRITE) {
            // No modo OVERWRITE, avança o head para descartar o item mais antigo
            rb->head = (rb->head + 1) % rb->capacity;
        }
    }

    rb->buffer[rb->tail] = *item;
    rb->tail = (rb->tail + 1) % rb->capacity;

    return RING_BUFFER_OK;
}

RingBufferStatus ring_buffer_pop(RingBuffer *rb, RingBufferItem *out_item) {
    if (rb == NULL || out_item == NULL) {
        return RING_BUFFER_ERR_INVALID_CAPACITY;
    }

    if (ring_buffer_is_empty(rb)) {
        return RING_BUFFER_ERR_EMPTY;
    }

    *out_item = rb->buffer[rb->head];
    rb->head = (rb->head + 1) % rb->capacity;

    return RING_BUFFER_OK;
}

void ring_buffer_clear(RingBuffer *rb) {
    if (rb != NULL) {
        rb->head = 0;
        rb->tail = 0;
    }
}
