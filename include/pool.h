// pool.h - Fixed block size, free list based, memory pool

#ifndef POOL_H
#define POOL_H

#include <complex.h>
#include <setjmp.h>
#include <stdint.h>

typedef struct pool_t {
  uint32_t block_n; // Block capacity
  uint32_t block_i; // Block count
  uint32_t block_s; // Block size in bytes
  void *data;       // Blocks
  void *free;       // Free chunk
} pool_t;

pool_t *pool_init(uint32_t, uint32_t);
void pool_fini(pool_t *);
void *pool_malloc(pool_t *);
void pool_free(pool_t *, void *);

#endif // POOL_H

#define POOL_IMPLEMENTATION
#ifdef POOL_IMPLEMENTATION // IMPLEMENTATION

#define POOL_MULTIPLIER_RESIZE 2
#define POOL_BLOCK_N 10

#include <stdio.h>
#include <stdlib.h>

static void pool_print(pool_t *pool);
static void pool_free_list(uintptr_t *, uint32_t, uint32_t);

// pool_init()
//
// Initializes a memory pool and returns a pointer to the memory pool
//
// block_s - The size of a block in bytes
// block_n - The number of initial blocks

pool_t *pool_init(uint32_t block_n, uint32_t block_s) {
  pool_t *pool = (pool_t *)malloc(sizeof(pool_t));

  // Set size and count
  pool->block_n = block_n;
  pool->block_s = block_s;

  pool->data = calloc(block_n, block_s);
  pool->free = pool->data;

  pool_free_list((uintptr_t *)pool->data, block_n, block_s);
  pool_print(pool);

  return pool;
}

// pool_malloc()
//
// Return a block of free memory
//
// pool - A pointer to the pool object

void *pool_malloc(pool_t *pool) {
  uintptr_t *free = (uintptr_t *)pool->free;

  if (free) {
    pool->free = (uintptr_t *)*free;
    pool->block_i++;
  }

  return free;
}

// pool_free()
//
// Free the memory at the pointer p
//
// pool - A pointer to the pool object

void pool_free(pool_t *pool, void *p) {
  // Convert the pointer into a usable type
  uintptr_t *free = (uintptr_t *)p;

  // Set the value at the address to the current front of the free list
  *free = (uintptr_t)pool->free;

  // Set the pool's free head to the newly freed pointer
  pool->free = (void *)free;

  pool->block_i--;
}

uint32_t pool_capacity(pool_t *pool) {
	return pool->block_n;
}

uint32_t pool_size(pool_t *pool) {
	return pool->block_i;
}

static void pool_print(pool_t *pool) {
  uintptr_t *c = (uintptr_t *)pool->free;
  while (((uintptr_t)c - (uintptr_t)pool->data) <
         pool->block_n * pool->block_s) {
    printf("%p %lx\n", c, *c);
    c++;
  }
}

static void pool_free_list(uintptr_t *block, uint32_t block_n,
                           uint32_t block_s) {
  uint8_t *ptr = (uint8_t *)block;
  for (int i = 0; i < block_n - 1; i++) {
    uintptr_t *tmp = (uintptr_t *)ptr;
    *tmp = (uintptr_t)ptr + block_s;
    ptr = ptr + block_s;
  }

  *(uintptr_t *)ptr = (uintptr_t)0;
}

#endif // POOL_IMPLEMENTATION
