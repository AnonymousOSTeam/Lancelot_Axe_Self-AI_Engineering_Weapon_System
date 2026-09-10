#ifndef MODULE_NAVIGATION_H
#define MODULE_NAVIGATION_H

#include "driver.h"

extern SystemDriver navigation_driver;

typedef struct {
    float coord_x;
    float coord_y;
    float speed;
} NavigationData;

#endif /* MODULE_NAVIGATION_H */
