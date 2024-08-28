#ifndef PUSHDOWN_H
#define PUSHDOWN_H

typedef pushdown_value_t stack_value_t;
#include "./stack.h"

typedef enum {
  END,
  PUSH,
  POP,
  SKIP,
  COPY,
  ADD,
  SUB,
  MUL,
  DIV,
  SIN,
  COS,
  TAN,
  ASIN,
  ACOS,
  ATAN,
  SINH,
  COSH,
  TANH,
  ASINH,
  ACOSH,
  ATANH,
  EXP,
  LOG,
  SQRT,
  ABS
} pushdown_instruction_t;

void pushdown_run(stack_t *, stack_t *, pushdown_instruction_t *);

#endif

#ifdef PUSHDOWN_IMPLEMENTATION

#include <tgmath.h>

void pushdown_run(stack_t *output, stack_t *input,
                  pushdown_instruction_t *instructions) {
  pushdown_instruction_t *op = instructions;

  pushdown_value_t x, y;

  while (*op) {
    switch (*op) {
    case PUSH:
      stack_push(output, stack_pop(input));
      break;
    case POP:
      stack_pop(output);
      break;
    case SKIP:
      stack_pop(input);
      break;
    case COPY:
      stack_push(output, stack_peek(output));
      break;
    case ADD:
      x = stack_pop(output);
      y = stack_pop(output);
      stack_push(output, x + y);
      break;
    case SUB:
      x = stack_pop(output);
      y = stack_pop(output);
      stack_push(output, x - y);
      break;
    case MUL:
      x = stack_pop(output);
      y = stack_pop(output);
      stack_push(output, x * y);
      break;
    case DIV:
      x = stack_pop(output);
      y = stack_pop(output);
      stack_push(output, x / y);
      break;
    case SIN:
      stack_push(output, sin(stack_pop(output)));
      break;
    case COS:
      stack_push(output, cos(stack_pop(output)));
      break;
    case TAN:
      stack_push(output, tan(stack_pop(output)));
      break;
    case ASIN:
      stack_push(output, asin(stack_pop(output)));
      break;
    case ACOS:
      stack_push(output, acos(stack_pop(output)));
      break;
    case ATAN:
      stack_push(output, atan(stack_pop(output)));
      break;
    case SINH:
      stack_push(output, sinh(stack_pop(output)));
      break;
    case COSH:
      stack_push(output, cosh(stack_pop(output)));
      break;
    case TANH:
      stack_push(output, tanh(stack_pop(output)));
      break;
    case ASINH:
      stack_push(output, asinh(stack_pop(output)));
      break;
    case ACOSH:
      stack_push(output, acosh(stack_pop(output)));
      break;
    case ATANH:
      stack_push(output, atanh(stack_pop(output)));
      break;
    case EXP:
      stack_push(output, exp(stack_pop(output)));
      break;
    case LOG:
      stack_push(output, log(stack_pop(output)));
      break;
    case SQRT:
      stack_push(output, sqrt(stack_pop(output)));
      break;
    case ABS:
      stack_push(output, fabs(stack_pop(output)));
      break;
    }

    op++;
  }
}

#endif