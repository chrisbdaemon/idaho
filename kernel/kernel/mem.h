#ifndef _KERNEL_MEM_H
#define _KERNEL_MEM_H

#include <stddef.h>
#include <kernel/multiboot.h>

#define BLOCK_SIZE  4096
#define BLOCK_COUNT 1024
enum blk_status {BLOCK_USED, BLOCK_FREE};
typedef enum blk_status blk_status_t;

void memory_initialize(multiboot_info_t *mbd);
void *kmalloc(size_t len);
void kfree(void *addr);

#endif
