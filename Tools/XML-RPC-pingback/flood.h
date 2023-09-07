#ifndef FLOOD_H
#define FLOOD_H

#include <pthread.h>
#include <time.h>

typedef struct attack_t {
  char* target;
  time_t duration;
  size_t threads;
} attack_t;

void flood_start(attack_t* attack);
static void* flood_worker(void* attack_data);

#endif // FLOOD_H
