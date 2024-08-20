typedef double stack_value_t;

#define STACK_IMPLEMENTATION
#include "../include/stack.h"

void main() {
    stack_t *stack = stack_init(10);

    for(int i = 0; i < 15; i++) {
        stack_push(stack, (stack_value_t) i);
    }
    
    printf("%lf\n", stack_peek(stack));
    printf("%lf\n", stack_pop(stack));

    for(int i = 0; i < 15; i++) {
        printf("%lf\n", stack_pop(stack));
    }
}