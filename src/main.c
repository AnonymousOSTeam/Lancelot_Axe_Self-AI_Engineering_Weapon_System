#include <stdio.h>
#include "device.h"
#include "module_defense.h"
#include "module_navigation.h"

#define MAX_DRIVERS 2

int main(void) {
    IntelligentDevice lancelot;
    init_device(&lancelot);
    print_device_status(&lancelot);

    SystemDriver *system_bus[MAX_DRIVERS] = {
        &defense_driver,
        &navigation_driver
    };

    printf("\n--- Inicializando Barramento de Drivers ---\n");
    for (int i = 0; i < MAX_DRIVERS; i++) {
        if (system_bus[i]->init) {
            system_bus[i]->init();
        }
    }

    printf("\n--- Executando Subrotinas ---\n");
    system_bus[0]->execute_subroutine(1, NULL);

    NavigationData target = { .coord_x = 105.4f, .coord_y = -32.8f, .speed = 1.5f };
    system_bus[1]->execute_subroutine(2, &target);

    printf("\n--- Desligamento do Sistema ---\n");
    for (int i = 0; i < MAX_DRIVERS; i++) {
        if (system_bus[i]->shutdown) {
            system_bus[i]->shutdown();
        }
    }

    return 0;
}
