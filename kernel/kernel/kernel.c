#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <kernel/tty.h>
#include <kernel/multiboot.h>

#include "dbg.h"
#include "mem.h"

extern int _kcode_start, _kcode_end;

void kernel_main(multiboot_info_t* mbd) {
  void *blk;

	terminal_initialize();
  printf("Terminal initialized\n");
  memory_initialize(mbd);
  printf("Memory initialized\n");

  printf("%d bytes being used by kernel\n",
         (unsigned int)&_kcode_end - (unsigned int)&_kcode_start);
}
