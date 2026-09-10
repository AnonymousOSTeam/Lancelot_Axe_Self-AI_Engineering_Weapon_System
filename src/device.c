#include <stdio.h>
#include "device.h"

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
