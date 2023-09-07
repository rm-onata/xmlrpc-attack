#ifndef CORE_H
#define CORE_H

#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#define MAX_PARAM_SZ 128

typedef struct args_t {
  const char* list_loc;
  const char* target;
  size_t threads;
  time_t duration;
} args_t;

#endif // CORE_H
