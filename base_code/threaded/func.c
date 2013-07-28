
static inline void timespec_add_us(struct timespec *t, uint64_t d)
{
    d *= 1000;
    d += t->tv_nsec;
    while (d >= NSEC_PER_SEC) {
        d -= NSEC_PER_SEC;
        t->tv_sec += 1;
    }
    t->tv_nsec = d;
}

static void wait_next_activation(periodic_task_t *pd)
{
    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &pd->r, NULL);
    timespec_add_us(&pd->r, pd->period);
}

int start_periodic_timer(uint64_t offs, int t, periodic_task_t *pd)
{
    clock_gettime(CLOCK_REALTIME, &pd->r);
    timespec_add_us(&pd->r, offs);
    pd->period = t;

    return 0;
}