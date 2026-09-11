#ifndef MODULE_DEFENSE_H
#define MODULE_DEFENSE_H

#include "driver.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Status dos eventos do módulo de defesa */
typedef enum {
    DEFENSE_OK = 0,
    DEFENSE_ERR_INTEGRITY_VIOLATION = -1,
    DEFENSE_ERR_UNAUTHORIZED_ACCESS = -2,
    DEFENSE_ERR_CRYPTO_FAIL	    = -3,
    DEFENSE_ERR_ARENA_EXHAUSTED	    = -4,
} DefenseStatus;

/* Estrutura de telemetria e integridade do processo */
typedef struct {
    uint32_t expected_code_hash;  /* Hash/CRC da seção de código carregada em memória */
    uint32_t total_scans;	   /* Contador de verificações de integridade realizadas */
    uint32_t violation_count;	   /* Contador de anomalias/violações de memória */
    bool self_healing_enabled;	   /* Flag para habilitar resposta automática/bloqueio */
} SecurityContext;

/* Funções de ciclo de vida do driver de defesa */
DefenseStatus module_defense_init(SystemDriver *driver, SecurityContext *ctx);
DefenseStatus module_defense_scan_integrity(const SecurityContext *ctx, const void *code_ptr, size_t code_len);
DefenseStatus module_defense_encrypt_payload(uint8_t *data, size_t len, uint32_t key_seed);
DefenseStatus module_defense_decrypt_payload(uint8_t *data, size_t len, uint32_t key_seed);
void module_defense_secure_wipe(void *v, size_t n);

#endif /* MODULE_DEFENSE_H */

