#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

#include "arena.h"
#include "ring_buffer.h"

#define ARENA_CAPACITY_BYTES (1024 * 4)
#define TEST_RING_CAPACITY   8

static void print_section(const char *title) {
    printf("\n=== %s ===\n", title);
}

int main(void) {
    printf("[LANCELOT AXE v3.0.0-dev] Iniciando Teste de Integração Arena + Ring Buffer\n");

    print_section("1. Teste da Arena de Memória");

    uint8_t backing_memory[ARENA_CAPACITY_BYTES];
    Arena arena;
    arena_init(&arena, backing_memory, ARENA_CAPACITY_BYTES);

    printf("[PASS] Arena inicializada. Capacidade: %zu bytes, Livre: %zu bytes\n",
           arena.capacity, arena_available(&arena));

    void *ptr1 = arena_alloc(&arena, 13);
    void *ptr2 = arena_alloc(&arena, 24);

    assert(ptr1 != NULL && ptr2 != NULL);
    assert(((uintptr_t)ptr1 % sizeof(void *)) == 0);
    assert(((uintptr_t)ptr2 % sizeof(void *)) == 0);

    printf("[PASS] Alocações de %d e %d bytes concluídas com alinhamento de %zu bytes.\n",
           13, 24, sizeof(void *));

    ArenaTemp snap = arena_temp_begin(&arena);
    size_t offset_before_temp = arena.offset;

    void *temp_ptr = arena_alloc(&arena, 256);
    assert(temp_ptr != NULL);
    printf("[INFO] Alocado bloco temporário de 256 bytes. Offset atual: %zu\n", arena.offset);

    arena_temp_end(snap);
    assert(arena.offset == offset_before_temp);
    printf("[PASS] Snapshot restaurado com sucesso. Offset retornado para: %zu\n", arena.offset);

    arena_reset(&arena);
    assert(arena.offset == 0);
    printf("[PASS] Reset total da Arena executado com sucesso.\n");

    print_section("2. Teste do Ring Buffer alocado na Arena");

    RingBuffer rb;
    RingBufferStatus status = ring_buffer_init_from_arena(&rb, &arena, TEST_RING_CAPACITY, RING_BUFFER_MODE_STRICT);

    assert(status == RING_BUFFER_OK);
    assert(ring_buffer_is_empty(&rb) == true);
    assert(ring_buffer_size(&rb) == 0);

    printf("[PASS] Ring Buffer (Capacidade: %zu) alocado na Arena. Espaço restante na Arena: %zu bytes.\n",
           rb.capacity, arena_available(&arena));

    for (uint32_t i = 1; i <= TEST_RING_CAPACITY; i++) {
        RingBufferItem item = {
            .event_id = i,
            .timestamp = 1000 + i,
            .payload_size = 8
        };
        snprintf((char *)item.payload, sizeof(item.payload), "EVT_%02u", i);

        status = ring_buffer_push(&rb, &item);
        assert(status == RING_BUFFER_OK);
    }

    assert(ring_buffer_is_full(&rb) == true);
    assert(ring_buffer_size(&rb) == TEST_RING_CAPACITY);
    printf("[PASS] Ring Buffer preenchido até capacidade máxima (%d/%d itens).\n",
           TEST_RING_CAPACITY, TEST_RING_CAPACITY);

    RingBufferItem overflow_item = { .event_id = 99 };
    status = ring_buffer_push(&rb, &overflow_item);
    assert(status == RING_BUFFER_ERR_FULL);
    printf("[PASS] Modo STRICT rejeitou corretamente inserção em buffer cheio.\n");

    print_section("3. Teste de Consumo (POP)");

    RingBufferItem out_item;
    status = ring_buffer_pop(&rb, &out_item);
    assert(status == RING_BUFFER_OK);
    assert(out_item.event_id == 1);
    printf("[PASS] Item lido com sucesso: ID=%u, Payload=\"%s\"\n", out_item.event_id, out_item.payload);

    assert(ring_buffer_is_full(&rb) == false);
    assert(ring_buffer_size(&rb) == (TEST_RING_CAPACITY - 1));

    print_section("4. Teste do Modo OVERWRITE");

    rb.mode = RING_BUFFER_MODE_OVERWRITE;

    RingBufferItem item_new1 = { .event_id = 90, .timestamp = 2001 };
    RingBufferItem item_new2 = { .event_id = 91, .timestamp = 2002 };

    ring_buffer_push(&rb, &item_new1);
    ring_buffer_push(&rb, &item_new2);

    printf("[PASS] Evento %u inserido em modo OVERWRITE.\n", item_new2.event_id);

    status = ring_buffer_pop(&rb, &out_item);
    assert(status == RING_BUFFER_OK);
    assert(out_item.event_id == 3);
    printf("[PASS] Evento lido corretamente após descarte por overwrite: ID=%u\n", out_item.event_id);

    ring_buffer_clear(&rb);
    assert(ring_buffer_is_empty(&rb) == true);
    printf("[PASS] Ring Buffer limpo com sucesso.\n");

    printf("\n>>> TODOS OS TESTES DE INTEGRAÇÃO DA PARTITION 3 FORAM CONCLUÍDOS COM SUCESSO! <<<\n");
    return 0;
}
