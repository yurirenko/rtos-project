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


#define MINOR_CYCLE 50

#define MAJOR_CYCLE 300

static uint8_t scheduling_table[] =  { 7, 1, 3, 5, 3, 1 };

#define NSEC_PER_SEC 1000000000ULL
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

static void wait_next_activation(struct periodic_task *t)
{
    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &t->r, NULL);
    timespec_add_us(&t->r, t->period);
}

struct periodic_task *start_periodic_timer(uint64_t offs, int t)
{
    struct periodic_task *p;

    p = malloc(sizeof(struct periodic_task));
    if (p == NULL) {
        return NULL;
    }

    clock_gettime(CLOCK_REALTIME, &p->r);
    timespec_add_us(&p->r, offs);
    p->period = t;

    return p;
}

void task1(void)
{
  int i,j;
 
  for (i=0; i<3; i++) {
    for (j=0; j<1000; j++) ;
    printf("1");
    fflush(stdout);
  }
}
void task2(void)
{
  int i,j;

  for (i=0; i<5; i++) {
    for (j=0; j<10000; j++) ;
    printf("2");
    fflush(stdout);
  }
}
void task3(void)
{
  static uint64_t previous;
  uint64_t t;
  struct timeval tv;

  if (previous == 0) {
    gettimeofday(&tv, NULL);
    previous = tv.tv_sec * 1000ULL + tv.tv_usec / 1000ULL;
  }

  gettimeofday(&tv, NULL);
  t = tv.tv_sec * 1000ULL + tv.tv_usec / 1000ULL;
  printf("\tT: %Lu\n", t - previous);
  previous = t;
}

int main(int argc, char *argv[])
{
    struct periodic_task *pt;
    int t = 0;

    pt = start_periodic_timer(1000000, MINOR_CYCLE * 1000);
    if (pt < 0) {
        perror("Start Periodic Timer");

        return -1;
    }

    while(1) {
        uint8_t scheduled_tasks;




        wait_next_activation(pt);

        scheduled_tasks = scheduling_table[t / MINOR_CYCLE];
				if (scheduled_tasks & 1)					
						task1();

				if (scheduled_tasks & 2)					
						task2();

				if (scheduled_tasks & 4)					
						task3();

				t = (t + MINOR_CYCLE)%MAJOR_CYCLE;
		}	 return 0;
}