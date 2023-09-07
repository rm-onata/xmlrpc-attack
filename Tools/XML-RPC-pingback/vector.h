#ifndef VECTOR_H
#define VECTOR_H

#include <string.h>
#include <time.h>

#define FOR_EACH(a,b) for(b = a; b != NULL; b = b->next_ptr)

struct node {
  unsigned char* host;
  unsigned char* path;
  struct node* next_ptr;
};

extern struct node* node_start;

void vector_push(char* node_data);
void vector_destroy();
int vector_size();
struct node* vector_rand();

#endif // VECTOR_H
