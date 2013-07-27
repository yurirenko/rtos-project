#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>


extern int clock_nanosleep(clockid_t __clock_id, int __flags,
               __const struct timespec *__req,
         struct timespec *__rem);


struct periodic_task {
  struct timespec r;
  int period;
};

