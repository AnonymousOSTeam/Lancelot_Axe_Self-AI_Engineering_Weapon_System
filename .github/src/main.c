#include <stdio.h>
#include "device.h"
#include "module_defense.h"
#include "module_navigation.h"

#define MAX_DRIVERS 2

int main(void) {
    IntelligentDevice lancelot;
    init_device(&lancelot);
    print_device_status(&lancelot);

    /* Vetor de drivers registrados no sistema */
    SystemDriver *system_bus[MAX_DRIVERS] = {
        &defense_driver,
        &navigation_driver
    };

    printf("\n--- Inicializando Barramento de Drivers ---\n");
    for (int i = 0; i < MAX_DRIVERS; i++) {
         if (system_bus[i]->init();

         }
    }

    printf("\n--- Executando Subrotinas ---\n");
    /* Teste de Defesa */
    system_bus[0]->execute_subroutine(1, NULL);

    printf("\n--- Desligamento do Sistema ---\n");
    for (int i = 0; i < MAX_DRIVERS; i++) {
        if (system_bus[i]->shutdown) {
            system_bus[i]->shutdown();
        }
    }

    return 0;
}
