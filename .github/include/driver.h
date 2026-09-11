#ifndef DRIVER_H
#define DRIVER_H

#include <stdint.h>

typedef struct {
    const char *name;
    int (*init)(void);
    int (*execute_subroutine)(uint32_t mode, void *data);
    void (*shutdown)(void);
} SystemDriver;

#endif /* DRIVER_H */

