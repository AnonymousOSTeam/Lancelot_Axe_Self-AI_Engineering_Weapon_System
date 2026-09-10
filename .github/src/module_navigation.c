#include <stdio.h>
#include "module_navigation.h"

static int nav_init(void) {
    printf("[DRIVER:NAV] Sistema de Navegação iniciada.\n");
    return 0;
}

static int nav_execute(uint32_t mode, void *data) {
    if (data != NULL) {
        NavigationData *nav_data = (NavigationData *)data;
        printf("[DRIVER:NAV] Mover para X: %.2f, Y: %.2f | Velocidade: %.2f (modo %u)\n",
               nav_data->coord_x, nav_data->coord_y, nav_data->speed, mode);
    } else {
        printf("[DRIVER:NAV] Navegação executada no modo %u (sem coordenadas).\n", mode);
    }
    return 0;
}

static void nav_shutdown(void) {
    printf("[DRIVER:NAV] Sistema de Navegação desligado.\n");
}

SystemDriver navigation_driver = {
     .name = "Navigation System",
     .init = nav_init,
     .execute_subroutine = nav_execute,
     .shutdown = nav_shutdown
};
