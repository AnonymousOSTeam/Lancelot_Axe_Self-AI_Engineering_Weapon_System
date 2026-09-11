#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> /* Dá acesso a 'bool', 'true', 'false' */
#include <stdint.h>  /* Dá acesso a 'uint32_t' */
#include <fcntl.h>

/* 1. Estrutura IntelligentDevice */
typedef struct {
    bool ai_core_online;
    /* Outros campos do dispositivo */
} IntelligentDevice;

/* 2. Estrutura SystemDriver */
typedef struct {
    int (*init)(void);
    int (*execute_subroutine)(uint32_t mode, void *data);
    void (*shutdown)(void);
} SystemDriver;

/* 3. Protótipos das funções do driver */
int defense_init(void);
int defense_trigger_shield(uint32_t mode, void *data);
void defense_cleanup(void);

/* 4. Instância do driver utilizando os protótipos */
SystemDriver defense_driver = {
    .init = defense_init,
    .execute_subroutine = defense_trigger_shield,
    .shutdown = defense_cleanup
};

/* 5. Funções de inicialização e status do dispositivo */
void init_device(IntelligentDevice *device) {
    if (device) {
        device->ai_core_online = true;
    }
}

void print_device_status(const IntelligentDevice *device) {
    if (device) {
        printf("AI Core Status      : %s\n", device->ai_core_online ? "ONLINE" : "OFFLINE");
    }
}

/* 6. Implementação das subrotinas do driver */
int defense_init(void) {
    printf("[DRIVER] Defense subsystem initialized.\n");
    return 0;
}

/* Implementado uma única vez com tratamento para o parâmetro 'data' não utilizado */
int defense_trigger_shield(uint32_t mode, void *data) {
    (void)data; /* Silencia o aviso de parâmetro não utilizado */

    printf("[DRIVER] Shield activated in mode %u.\n", mode);
    return 0;
}

void defense_cleanup(void) {
    printf("[DRIVER] Defense subsystem shutdown.\n");
}

/* 7. Função Principal (main) */
int main(void) {
    IntelligentDevice lancelot;
    init_device(&lancelot);
    print_device_status(&lancelot);

    defense_driver.init();
    defense_driver.execute_subroutine(1, NULL);
    defense_driver.shutdown();

    return 0;
}
