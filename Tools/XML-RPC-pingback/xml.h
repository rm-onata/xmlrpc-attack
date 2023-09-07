#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>

#ifdef DEBUG
  #include <errno.h>
#endif

#define TRUE 1
#define FALSE 0

#define ZERO_BUF(a) memset(a, 0, sizeof(a))

typedef int BOOL;
