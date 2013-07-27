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
