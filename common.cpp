//
// Created by ac on 15-3-22.
//

#include "common.h"

unsigned long GetTickCount() {
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);

}

