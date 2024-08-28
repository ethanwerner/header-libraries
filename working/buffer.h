#ifndef BUFFER_H
#define BUFFER_H

#include "./type.h"

void *buffer_malloc(uint_t num, uint_t size);
void buffer_free(void *);
void buffer_insert(void *);
void buffer_remove(void *);

#endif

#ifdef BUFFER_IMPLEMENTATION

#define _buffer_from_meta(x) ((void *)((u8 *)x) + sizeof(buffer_meta_t))
#define _buffer_to_meta(x) ((void *)((u8 *)x) + sizeof(buffer_meta_t))

struct {
  uint_t i; // Array size
  uint_t n; // Array capacity
  uint_t s; // Element size
} buffer_meta_t;

void *buffer_malloc(uint_t n, uint_t s) {
  void *x = malloc((n * s) + sizeof(uint_t) * 3);
  buffer_meta_t *meta = (buffer_meta_t *)x;

  meta->i = 0;
  meta->n = n;
  meta->s = s;

  void *buffer = _buffer_from_meta(x);

  return buffer;
}

void buffer_free(void *buffer) { free(_buffer_to_meta(buffer)); }

uint_t buffer_size(void *buffer) {
  buffer_meta_t *meta = _buffer_to_meta(buffer);
  return meta->i;
}

uint_t buffer_capacity(void *buffer) {
  buffer_meta_t *meta = _buffer_to_meta(buffer);
  return meta->n;
}

void *buffer_resize(void *buffer) {
  buffer_meta_t *meta = _buffer_to_meta(buffer);

  if (meta->n > 32) {
    fp_t ratio = (fp_t)meta->i / (fp_t)meta->n;

    if (ratio > 0.8) {
      meta->n *= 2;
      meta = realloc(meta, sizeof(buffer_meta_t) + meta->n * meta->s)
    } else if (ratio < 0.2) {
      meta->n /= 2;
      meta = realloc(meta, sizeof(buffer_meta_t) + meta->n * meta->s)
    }
  }

  return meta;
}

#endif