    int pmin;
    pthread_attr_t attrs;
    struct sched_param sp;

    pthread_attr_init(&attrs);
    err = pthread_attr_setinheritsched(&attrs, PTHREAD_EXPLICIT_SCHED);
    if (err != 0) {
      perror("setineheritsched");
    }
    err = pthread_attr_setschedpolicy(&attrs, SCHED_FIFO);
    if (err != 0) {
      perror("setschedpolicy");
    }
    pmin = sched_get_priority_min(SCHED_FIFO);
