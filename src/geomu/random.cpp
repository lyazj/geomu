#include "geomu/random.h"
#include <time.h>
#include <unistd.h>

#include "geomu/diagnostic.h"

static size_t get_random_seed()
{
  static time_t start_time = time(NULL);  // Initialized once and only once, synchronized.
  pid_t tid = gettid();
  size_t seed = start_time ^ tid;  // Unique for each thread.
  difo << "random seed for thread " << tid << ": " << seed << dend;
  return seed;
}

thread_local __typeof(thread_random_engine) thread_random_engine(get_random_seed());
