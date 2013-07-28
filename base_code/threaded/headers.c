#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>

extern int clock_nanosleep(clockid_t __clock_id, int __flags,
                           __const struct timespec *__req,
                           struct timespec *__rem);

typedef struct periodic_task {
        struct timespec r;
        int period;
} periodic_task_t;

#define NSEC_PER_SEC 1000000000ULL


