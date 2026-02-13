#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include "timing.h"

#include <errno.h>

#define NANOSECONDS_PER_SECOND 1000000000L
#define NANOSECONDS_PER_MILLISECOND 1000000L

static int timespec_compare(struct timespec a, struct timespec b) {
    if (a.tv_sec < b.tv_sec) return -1;
    if (a.tv_sec > b.tv_sec) return 1;
    if (a.tv_nsec < b.tv_nsec) return -1;
    if (a.tv_nsec > b.tv_nsec) return 1;
    return 0;
}

static struct timespec timespec_add_ns(struct timespec t, long ns) {
    t.tv_nsec += ns;
    while (t.tv_nsec >= NANOSECONDS_PER_SECOND) {
        t.tv_nsec -= NANOSECONDS_PER_SECOND;
        t.tv_sec += 1;
    }
    return t;
}

static struct timespec timespec_sub(struct timespec end, struct timespec start) {
    struct timespec diff;
    diff.tv_sec = end.tv_sec - start.tv_sec;
    diff.tv_nsec = end.tv_nsec - start.tv_nsec;
    if (diff.tv_nsec < 0) {
        diff.tv_nsec += NANOSECONDS_PER_SECOND;
        diff.tv_sec -= 1;
    }
    if (diff.tv_sec < 0) {
        diff.tv_sec = 0;
        diff.tv_nsec = 0;
    }
    return diff;
}

int timing_init(TickTimer* timer, int tick_ms) {
    if (timer == NULL || tick_ms <= 0) return 0;

    if (clock_gettime(CLOCK_MONOTONIC, &timer->next_tick) != 0) return 0;

    timer->interval_ns = (long)tick_ms * NANOSECONDS_PER_MILLISECOND;
    if (timer->interval_ns <= 0) return 0;

    timer->next_tick = timespec_add_ns(timer->next_tick, timer->interval_ns);
    timer->started = 1;
    return 1;
}

void timing_sleep_until_next(TickTimer* timer) {
    if (timer == NULL || !timer->started) return;

    struct timespec now;
    if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) return;

    if (timespec_compare(now, timer->next_tick) < 0) {
        struct timespec remaining = timespec_sub(timer->next_tick, now);
        while (nanosleep(&remaining, &remaining) == -1) {
            if (errno != EINTR) break;
        }

        if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) return;
    }

    do {
        timer->next_tick = timespec_add_ns(timer->next_tick, timer->interval_ns);
    } while (timespec_compare(timer->next_tick, now) <= 0);
}
