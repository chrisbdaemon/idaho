#include "mem.h"
#include "dbg.h"

#include <stdint.h>

static blk_status_t kmem_bitmap[BLOCK_COUNT];

typedef struct {
  uint32_t size;
} block_t __attribute__ ((__packed__));

#define MALLOC_HEADER_SIZE 4

void *bitmap_idx_to_addr(uint16_t idx);
uint16_t bitmap_addr_to_idx(void *addr);
size_t len_to_blocks(size_t len);

void memory_initialize(multiboot_info_t *mbd) {
  if ((mbd->flags & (1 << 6)) != 0) {
    blk_status_t status;
    memory_map_t *mmap = (memory_map_t *)(mbd->mmap_addr);
    while ((unsigned long)mmap < mbd->mmap_addr + mbd->mmap_length) {
      if (mmap->type == 1 && mmap->base_addr_low != 0x00000000)
        status = BLOCK_FREE;
      else
        status = BLOCK_USED;

      mark_block((void*)mmap->base_addr_low, mmap->length_low, status);

      mmap = (memory_map_t *)((unsigned int)mmap + mmap->size + sizeof(mmap->size));
    }
  }
}

void *kmalloc(size_t length) {
  block_t *block;
  size_t blk_cnt;

  blk_cnt = len_to_blocks(length + MALLOC_HEADER_SIZE);

  block = (block_t*)0x0;
  for (uint16_t i = 0; i < BLOCK_COUNT; i++) {
    if (kmem_bitmap[i] == BLOCK_USED)
      continue;

    int block_free = BLOCK_FREE;
    for (uint16_t j = 1; j < blk_cnt && block_free == BLOCK_FREE; j++) {
      if (kmem_bitmap[i + j] == BLOCK_USED)
        block_free = BLOCK_USED;
    }
    if (block_free == BLOCK_FREE) {
      block = (block_t*)bitmap_idx_to_addr(i);
      break;
    }
  }

  if (block != 0) {
    block->size = (uint32_t)blk_cnt;
    block = (void*)((char*)block + 4); // account for offset
    mark_block(block, length, BLOCK_USED);
  }

  return (void*)block;
}

void kfree(void *addr) {
  block_t *block;
  block = (block_t*)((char*)addr - 4);
  mark_block((void*)block, block->size * BLOCK_SIZE, BLOCK_FREE);
}

size_t len_to_blocks(size_t len) {
  size_t blk_cnt;

  if (len < BLOCK_SIZE)
    return 1;
  blk_cnt = len / BLOCK_SIZE;
  if (blk_cnt * BLOCK_SIZE < len)
    blk_cnt++;

  return blk_cnt;
}

void mark_block(void *addr, size_t len, blk_status_t status) {
  uint16_t idx;
  idx = bitmap_addr_to_idx(addr);
  len = len_to_blocks(len);

  for (size_t i = 0; i < len; i++)
    kmem_bitmap[idx + i] = status;
}

void *bitmap_idx_to_addr(uint16_t idx) {
  char *addr = 0;
  addr += idx * BLOCK_SIZE;
  return (void*)addr;
}

uint16_t bitmap_addr_to_idx(void *addr) {
  uint32_t idx;
  idx = ((char *)addr - (char*)0x0) / BLOCK_SIZE;
  return (uint16_t)idx;
}
