#include "geomu/random.h"
#include <time.h>
#include <unistd.h>

static time_t start_time = time(NULL);
thread_local std::default_random_engine thread_random_engine(gettid() ^ start_time);
