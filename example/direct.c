typedef double value_t;
#define DIRECT_IMPLEMENTATION
#include "../include/direct.h"
#include <stdio.h>

value_t one(node_t *node) {
  return 1;
}

value_t add(node_t *node) {
  edge_t *edge = node->edge;
  value_t x = node->x;

  while(edge) {
    x += edge->node->x;
    edge = edge->next;
  }

  return x;
}

value

int main() {
  node_t x0 = {.x = 0.0, .function = add};
  node_t x1 = {.x = 0.0, .function = one};
  node_t x2 = {.x = 0.0, .function = add};
  node_t x3 = {.x = 0.0, .function = add};
  node_t x4 = {.x = 0.0, .function = add};
  node_t x5 = {.x = 0.0, .function = add};
  node_t x6 = {.x = 0.0, .function = one};

  printf("x0: %p\n", &x0);
  printf("x1: %p\n", &x1);
  printf("x2: %p\n", &x2);
  printf("x3: %p\n", &x3);
  printf("x4: %p\n", &x4);
  printf("x5: %p\n", &x5);
  printf("x6: %p\n", &x6);

  direct_connect(&x0, &x2);
  direct_connect(&x0, &x1);
  direct_connect(&x0, &x3);
  direct_connect(&x2, &x1);
  direct_connect(&x2, &x4);
  direct_connect(&x3, &x4);
  direct_connect(&x4, &x5);
  direct_connect(&x4, &x6);
  direct_connect(&x5, &x2);

  direct_update(&x0);
  printf("data: %lf\n", x0.x);
  direct_update(&x0);
  printf("data: %lf\n", x0.x);
  direct_update(&x0);
  printf("data: %lf\n", x0.x);

  return 0;
}