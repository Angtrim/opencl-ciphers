#ifndef BENCHMARK_H
#define BENCHMARK_H


static
void printdiff(struct timespec *begin, struct timespec *end, const char *msg) {
  time_t delta_sec;
  long delta_nsec;

  // tv_* are signed types! (don't ask me why)
  if (end->tv_nsec - begin->tv_nsec < 0)  {
    delta_sec = end->tv_sec - begin->tv_sec - 1;
    delta_nsec = 1000000000 + end->tv_nsec - begin->tv_nsec;
  } else {
    delta_sec = end->tv_sec - begin->tv_sec;
    delta_nsec = end->tv_nsec - begin->tv_nsec;
  }

  fprintf(stderr, "%s\t%lds+%010ldns\n", msg, delta_sec, delta_nsec);
}

#  define BEGIN_KEYSCHED fprintf(stderr, "BEGIN keysched\n")

#  define END_KEYSCHED fprintf(stderr, "END keysched\n")

// begin must be in the parent scope, so no do{}while sorry
#  define BEGIN_ENCRYPT(_) \
     struct timespec begin; \
     clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &begin);

#  define END_ENCRYPT(_) \
     do { \
       struct timespec end; \
       clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end); \
       printdiff(&begin, &end, "encrypt"); \
     } while(0)


#  define DO_BENCHMARK(name) \
     fill_random_pool(); \
     struct timespec res; \
     clock_getres(CLOCK_PROCESS_CPUTIME_ID, &res); \
     fprintf(stderr, "%s\n\nClock resolution \t%lds+%ldns\n\n", \
                     name, res.tv_sec, res.tv_nsec); \
     char *txtbenches = getenv("BENCH_TIMES"); \
     long nbenches = txtbenches? strtol(txtbenches, 0, 10) : 10000; \
     for (int benchno = 0; benchno < nbenches; ++benchno)

#  define PASS printf("OK\n");
#  define FAIL printf("NO\n");

#define LET_MAGICO '1'

#endif
