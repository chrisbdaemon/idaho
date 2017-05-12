#include <stdio.h>

#include "dbg.h"

#define DUMP_WIDTH 16

void dump_mem(const void *ptr, size_t len) {
  size_t i = 0;
  char *buf = (char *)ptr;
  printf("%p ", ptr);
  for (i = 0; i < len; i++) {
    if (i != 0 && i % DUMP_WIDTH == 0)
      printf("\n%p ", &buf[i]);
    printf("%x ", buf[i]);
  }
  printf("\n");
}
