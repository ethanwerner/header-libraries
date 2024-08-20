#ifndef STACK_H
#define STACK_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef uint64_t uint_t;
typedef int64_t int_t;
typedef double fp_t;

typedef struct stack {
  uint_t size;
  uint_t capacity;
  stack_value_t *zero;
  stack_value_t *data;
  stack_value_t *head;
} stack_t;

stack_t *stack_init(uint_t);
void stack_push(stack_t *, stack_value_t);
stack_value_t stack_pop(stack_t *);
stack_value_t stack_peek(stack_t *);

#endif

#ifdef STACK_IMPLEMENTATION

stack_t *stack_init(uint_t capacity) {
  stack_t *stack = calloc(1, sizeof(stack_t));

  stack->size = 0;
  stack->capacity = capacity;
  stack->zero = calloc(1, sizeof(stack_value_t));
  stack->data = calloc(capacity, sizeof(stack_value_t));
  stack->head = stack->data;

  return stack;
}

void stack_push(stack_t *stack, stack_value_t x) {
  if (stack->size == stack->capacity) {
    stack->capacity *= 2;
    stack->data = realloc(stack->data, sizeof(stack_value_t) * stack->capacity);
    stack->head = &(stack->data[stack->size - 1]);
    
  }

  stack->data[stack->size] = x;
  stack->size++;
  stack->head++;
}

stack_value_t stack_pop(stack_t *stack) {
    if(stack->size == 0) {
       return *(stack->zero); 
    }

    stack_value_t x = *(stack->head);
    stack->size--;
    stack->head--;

    return x;
}

stack_value_t stack_peek(stack_t *stack) {
    if(stack->size == 0) {
       return *(stack->zero); 
    }    

    return *(stack->head);
}

#endif