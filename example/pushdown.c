typedef double pushdown_value_t;
#define STACK_IMPLEMENTATION
#define PUSHDOWN_IMPLEMENTATION
#include "../include/pushdown.h"

int main() {
  pushdown_instruction_t instructions[] = {PUSH, COPY, MUL, END};

  stack_t input = {};
  stack_t output = {};

  stack_init(&input);
  stack_init(&output);

  stack_push(&input, 4.0);

  pushdown_run(&output, &input, instructions);

  while (output.size) {
    printf("%lf\n", stack_pop(&output));
  }
}