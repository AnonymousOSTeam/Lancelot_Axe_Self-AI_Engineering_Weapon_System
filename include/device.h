#ifndef DEVICE_H
#define DEVICE_H

#include <stdbool.h>

typedef struct {
    bool ai_core_online;
} IntelligentDevice;

void init_device(IntelligentDevice *device);
void print_device_status(const IntelligentDevice *device);

#endif /* DEVICE_H */
