#ifndef TIMING_H
#define TIMING_H

#include <time.h>

typedef struct TickTimer {
    struct timespec next_tick;
    long interval_ns;
    int started;
} TickTimer;

int timing_init(TickTimer* timer, int tick_ms);
void timing_sleep_until_next(TickTimer* timer);

#endif
