#ifndef STACK_H
#define STACK_H

#include "./type.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stack {
  stack_value_t *data;
  stack_value_t *head;
  uint_t size;
  uint_t capacity;
} stack_t;

void stack_init(stack_t *);
stack_t *stack_from_array(stack_value_t *, uint_t);
void stack_push(stack_t *, stack_value_t);
stack_value_t stack_pop(stack_t *);
stack_value_t stack_peek(stack_t *);

#endif

#ifdef STACK_IMPLEMENTATION

const stack_value_t ZERO = 0;

void stack_init(stack_t *stack) {
  if (stack->capacity == 0) {
    stack->capacity = 16;
  }

  stack->size = 0;
  stack->data = calloc(stack->capacity, sizeof(stack_value_t));
  stack->head = stack->data;
}

stack_t *stack_from_array(stack_value_t *array, uint_t size) {
  stack_t *stack = calloc(1, sizeof(stack_t));

  stack->size = size;
  stack->capacity = size;
  stack->data = calloc(size, sizeof(stack_value_t));
  memcpy(stack->data, array, sizeof(stack_value_t) * size);
  stack->head = stack->data;

  return stack;
}

void stack_push(stack_t *stack, stack_value_t x) {
  if (stack->size == stack->capacity) {
    stack->capacity *= 2;
    stack->data = realloc(stack->data, sizeof(stack_value_t) * stack->capacity);
    stack->head = &(stack->data[stack->size - 1]);
  }

  *stack->head = x;
  stack->head++;
  stack->size++;
}

stack_value_t stack_pop(stack_t *stack) {
  if (stack->size == 0) {
    return ZERO;
  }

  stack_value_t x = stack->head[-1];
  stack->size--;
  stack->head--;

  return x;
}

stack_value_t stack_peek(stack_t *stack) {
  if (stack->size == 0) {
    return ZERO;
  }

  return stack->head[-1];
}

#endif